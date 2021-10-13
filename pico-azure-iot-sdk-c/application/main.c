#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/util/datetime.h"
#include "pico/binary_info.h"
#include "pico/critical_section.h"
#include "hardware/spi.h"
#include "hardware/pll.h"
#include "hardware/clocks.h"
#include "hardware/structs/pll.h"
#include "hardware/structs/clocks.h"

#include "wizchip_conf.h"
#include "dhcp.h"
#include "dns.h"
#include "netif.h"

#include "azure_samples.h"

const uint LED_PIN = PICO_DEFAULT_LED_PIN;
/* SPI */
#define SPI_PORT spi0

#define PIN_SCK 18
#define PIN_MOSI 19
#define PIN_MISO 16
#define PIN_CS 17
#define PIN_RST 20

/* Buffer */
#define ETHERNET_BUF_MAX_SIZE (1024 * 2)

/* Socket */
#define SOCKET_DHCP 0
#define SOCKET_DNS 1

/* Retry count */
#define DHCP_RETRY_COUNT 5
#define DNS_RETRY_COUNT 5
/* Critical section */
static critical_section_t g_wizchip_cri_sec;
/* Network */
static wiz_NetInfo g_net_info =
    {
        .mac = {0x00, 0x08, 0xDC, 0x12, 0x34, 0x11}, // MAC address
        .ip = {192, 168, 3, 111},                     // IP address
        .sn = {255, 255, 255, 0},                    // Subnet Mask
        .gw = {192, 168, 3, 1},                     // Gateway
        .dns = {8, 8, 8, 8},                         // DNS server
        // .dhcp = NETINFO_DHCP                         // DHCP enable/disable
        .dhcp = NETINFO_STATIC
};
static uint8_t g_ethernet_buf[ETHERNET_BUF_MAX_SIZE] = {
    0,
}; // common buffer

/* DHCP */
static uint8_t g_dhcp_get_ip_flag = 0;

/* DNS */
static uint8_t g_dns_target_domain[] = "www.wiznet.io";
static uint8_t g_dns_target_ip[4] = {
    0,
};
static uint8_t g_dns_get_ip_flag = 0;

/* Timer */
static struct repeating_timer g_timer;
static volatile uint16_t g_msec_cnt = 0;

static inline void wizchip_select(void);
static inline void wizchip_deselect(void);
static void wizchip_reset(void);
static uint8_t wizchip_read(void);
static void wizchip_write(uint8_t tx_data);
#ifdef USE_SPI_DMA
static void wizchip_read_burst(uint8_t *pBuf, uint16_t len);
static void wizchip_write_burst(uint8_t *pBuf, uint16_t len);
#endif
static void wizchip_critical_section_lock(void);
static void wizchip_critical_section_unlock(void);
static void wizchip_initialize(void);
static void wizchip_check(void);

/* Network */
static void network_initialize(void);
static void print_network_information(void);

/* DHCP */
static void wizchip_dhcp_init(void);
static void wizchip_dhcp_assign(void);
static void wizchip_dhcp_conflict(void);

/* Timer */
static bool repeating_timer_callback(struct repeating_timer *t);

void measure_freqs(void) {
    uint f_pll_sys = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_SYS_CLKSRC_PRIMARY);
    uint f_pll_usb = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_USB_CLKSRC_PRIMARY);
    uint f_rosc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_ROSC_CLKSRC);
    uint f_clk_sys = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS);
    uint f_clk_peri = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_PERI);
    uint f_clk_usb = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_USB);
    uint f_clk_adc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_ADC);
    uint f_clk_rtc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_RTC);
 
    printf("pll_sys  = %dkHz\n", f_pll_sys);
    printf("pll_usb  = %dkHz\n", f_pll_usb);
    printf("rosc     = %dkHz\n", f_rosc);
    printf("clk_sys  = %dkHz\n", f_clk_sys);
    printf("clk_peri = %dkHz\n", f_clk_peri);
    printf("clk_usb  = %dkHz\n", f_clk_usb);
    printf("clk_adc  = %dkHz\n", f_clk_adc);
    printf("clk_rtc  = %dkHz\n", f_clk_rtc);
 
// pll_sys  = 125000kHz
// pll_usb  = 48000kHz
// rosc     = 5180kHz
// clk_sys  = 125000kHz
// clk_peri = 125000kHz
// clk_usb  = 48000kHz
// clk_adc  = 48000kHz
// clk_rtc  = 47kHz
    // Can't measure clk_ref / xosc as it is the ref
}

int main() 
{
    uint8_t retval = 0;
    uint8_t dhcp_retry = 0;
    uint8_t dns_retry = 0;

//-----------------------------------------------------------------------------------
// Pico board configuration - W5100S, GPIO, Timer Setting
//-----------------------------------------------------------------------------------
    stdio_init_all();
    sleep_ms(3000); // wait for 3 second
    measure_freqs();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    // Enable SPI 0 at 1 MHz and connect to GPIOs
    spi_init(SPI_PORT, 5000 * 1000);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    // Make the SPI pins available to picotool
    bi_decl(bi_3pins_with_func(PIN_MISO, PIN_MOSI, PIN_SCK, GPIO_FUNC_SPI));
    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(PIN_CS);
    gpio_put(PIN_CS, 1);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    // Make the CS pin available to picotool
    bi_decl(bi_1pin_with_name(PIN_CS, "W5x00 CHIP SELECT"));
    critical_section_init(&g_wizchip_cri_sec);

    wizchip_reset();
    wizchip_initialize();
    wizchip_check();

    // add_repeating_timer_us(-1000, repeating_timer_callback, NULL, &g_timer);
    // DNS_init(SOCKET_DNS, g_ethernet_buf);

    if (g_net_info.dhcp == NETINFO_DHCP) // DHCP
    {
        wizchip_dhcp_init();
    }
    else // static
    {
        network_initialize();
        /* Get network information */
        print_network_information();
    }

    struct repeating_timer timer;
    add_repeating_timer_ms(-1000, repeating_timer_callback, NULL, &timer);
    // bool cancelled = cancel_repeating_timer(&timer);
    // printf("cancelled... %d\n", cancelled);

    // while (1);
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// CALL Main Funcion - Azure IoT SDK example funcion
// Select one application.
//-----------------------------------------------------------------------------------

    iothub_ll_telemetry_sample();
    //iothub_ll_c2d_sample();
    //iothub_ll_client_x509_sample();
    //prov_dev_client_ll_sample();

//-----------------------------------------------------------------------------------

    /* Infinite loop */
    for( ;; )
    {
        gpio_put(LED_PIN, 0);
        sleep_ms(2000); // wait for 
        gpio_put(LED_PIN, 1);
        sleep_ms(2000); // wait for 
    }
}

//-----------------------------------------------------------------------------------
// static functions 
//-----------------------------------------------------------------------------------
/* W5x00 */
static inline void wizchip_select(void)
{
    gpio_put(PIN_CS, 0);
}

static inline void wizchip_deselect(void)
{
    gpio_put(PIN_CS, 1);
}

static void wizchip_reset(void)
{
    gpio_set_dir(PIN_RST, GPIO_OUT);

    gpio_put(PIN_RST, 0);
    sleep_ms(1000);

    gpio_put(PIN_RST, 1);
    sleep_ms(1000);

    bi_decl(bi_1pin_with_name(PIN_RST, "W5x00 RESET"));
}

static uint8_t wizchip_read(void)
{
    uint8_t rx_data = 0;
    uint8_t tx_data = 0xFF;

    spi_read_blocking(SPI_PORT, tx_data, &rx_data, 1);

    return rx_data;
}

static void wizchip_write(uint8_t tx_data)
{
    spi_write_blocking(SPI_PORT, &tx_data, 1);
}

static void wizchip_critical_section_lock(void)
{
    critical_section_enter_blocking(&g_wizchip_cri_sec);
}

static void wizchip_critical_section_unlock(void)
{
    critical_section_exit(&g_wizchip_cri_sec);
}

static void wizchip_initialize(void)
{
    /* Deselect the FLASH : chip select high */
    wizchip_deselect();

    /* CS function register */
    reg_wizchip_cs_cbfunc(wizchip_select, wizchip_deselect);

    /* SPI function register */
    reg_wizchip_spi_cbfunc(wizchip_read, wizchip_write);
#ifdef USE_SPI_DMA
    reg_wizchip_spiburst_cbfunc(wizchip_read_burst, wizchip_write_burst);
#endif
    reg_wizchip_cris_cbfunc(wizchip_critical_section_lock, wizchip_critical_section_unlock);

    uint8_t temp;
    /* Check PHY link status */
    do
    {
        if (ctlwizchip(CW_GET_PHYLINK, (void *)&temp) == -1)
        {
            printf(" Unknown PHY link status\n");
            return;
        }
    } while (temp == PHY_LINK_OFF);
}

static void wizchip_check(void)
{
    /* Read version register */
    while (getVER() != 0x51) // W5100S
    {
        printf(" ACCESS ERR : VERSIONR != 0x51, read value = 0x%02x\n", getVER());
    }
}

/* Network */
static void network_initialize(void)
{
    ctlnetwork(CN_SET_NETINFO, (void *)&g_net_info);
}

static void print_network_information(void)
{
    uint8_t tmp_str[8] = {
        0,
    };

    ctlnetwork(CN_GET_NETINFO, (void *)&g_net_info);
    ctlwizchip(CW_GET_ID, (void *)tmp_str);

    if (g_net_info.dhcp == NETINFO_DHCP)
    {
        printf("====================================================================================================\n");
        printf(" %s network configuration : DHCP\n\n", (char *)tmp_str);
    }
    else
    {
        printf("====================================================================================================\n");
        printf(" %s network configuration : static\n\n", (char *)tmp_str);
    }

    printf(" MAC         : %02X:%02X:%02X:%02X:%02X:%02X\n", g_net_info.mac[0], g_net_info.mac[1], g_net_info.mac[2], g_net_info.mac[3], g_net_info.mac[4], g_net_info.mac[5]);
    printf(" IP          : %d.%d.%d.%d\n", g_net_info.ip[0], g_net_info.ip[1], g_net_info.ip[2], g_net_info.ip[3]);
    printf(" Subnet Mask : %d.%d.%d.%d\n", g_net_info.sn[0], g_net_info.sn[1], g_net_info.sn[2], g_net_info.sn[3]);
    printf(" Gateway     : %d.%d.%d.%d\n", g_net_info.gw[0], g_net_info.gw[1], g_net_info.gw[2], g_net_info.gw[3]);
    printf(" DNS         : %d.%d.%d.%d\n", g_net_info.dns[0], g_net_info.dns[1], g_net_info.dns[2], g_net_info.dns[3]);
    printf("====================================================================================================\n\n");
}

/* DHCP */
static void wizchip_dhcp_init(void)
{
    printf(" DHCP client running\n");
    DHCP_init(SOCKET_DHCP, g_ethernet_buf);
    reg_dhcp_cbfunc(wizchip_dhcp_assign, wizchip_dhcp_assign, wizchip_dhcp_conflict);
}

static void wizchip_dhcp_assign(void)
{
    getIPfromDHCP(g_net_info.ip);
    getGWfromDHCP(g_net_info.gw);
    getSNfromDHCP(g_net_info.sn);
    getDNSfromDHCP(g_net_info.dns);

    g_net_info.dhcp = NETINFO_DHCP;

    /* Network initialize */
    network_initialize(); // apply from DHCP

    print_network_information();
    printf(" DHCP leased time : %ld seconds\n", getDHCPLeasetime());
}

static void wizchip_dhcp_conflict(void)
{
    printf(" Conflict IP from DHCP\n");
    // halt or reset or any...
    while (1)
        ; // this example is halt.
}

/* Timer */
static bool repeating_timer_callback(struct repeating_timer *t)
{
    // printf("Repeat at %lld\n", time_us_64());

    // g_msec_cnt++;
    // if (g_msec_cnt >= 1000 - 1)
    // {
    //     g_msec_cnt = 0;

    //     DHCP_time_handler();
    //     DNS_time_handler();
    // }
    return true;
}
