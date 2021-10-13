// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>     
#include <time.h>
#include "wizchip_conf.h"
#include "dns.h"
#include "sntp.h"
#include <stdbool.h>
#include "netif.h"
#include "pico/stdlib.h"

static uint8_t g_ethernet_buf[1500] = {
    0,
}; // common buffer

#define SOCKET_DNS 3
static uint8_t dns_ip[4] = {8, 8, 8, 8};                         // DNS server

static uint32_t current_timestamp;
static uint8_t time_zone = 40; // Korea
#define SNTP_SERVER_ADDRESS "pool.ntp.org"
/* Timezone */
#define TIMEZONE 40 // Korea
static uint8_t g_sntp_server_ip[4] = {0, };

uint8_t wizchip_gethostbyname(const char* host, uint8_t* ip)
{
    uint8_t ret = 0;

    if (host[0] == '\0') {
        ret = -1;
    }
    else {
        DNS_init(SOCKET_DNS, g_ethernet_buf);

        ret = DNS_run(dns_ip, (uint8_t*)host, ip);
    //  * @return  -1 : failed. @ref MAX_DOMIN_NAME is too small \n
    //  *           0 : failed  (Timeout or Parse error)\n
    //  *           1 : success    
        
        if (ret)
        {
            printf("DNS success\n");
            printf("DNS target domain : %s\n", host);
            printf("DNS IP of target domain : %d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);
        }
        else
        {
            printf(" DNS failed\n");
        }
    }
    return ret;
}

void wizchip_sntp_init(void)
{
    printf("Initializing SNTP\n");
    uint8_t gettingip[4] = {0,};
    int rc = 0;
    rc = wizchip_gethostbyname(SNTP_SERVER_ADDRESS, g_sntp_server_ip);
    if (rc) {
        printf("Getting SNTP server pool.ntp.org ip: %d.%d.%d.%d\n", g_sntp_server_ip[0], g_sntp_server_ip[1], g_sntp_server_ip[2], g_sntp_server_ip[3]);
    }
    else {
        printf("Fail to getting SNTP server, set the time.google.com 216.239.35.8\n");
        // temporary assign - time.google.com
        g_sntp_server_ip[0] = 216;
        g_sntp_server_ip[1] = 239;
        g_sntp_server_ip[2] = 35;
        g_sntp_server_ip[3] = 8;
    }
    SNTP_init(SOCKET_DNS, g_sntp_server_ip, TIMEZONE, g_ethernet_buf);
/*
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_init();
*/
}

// /* Timezone */
// #define TIMEZONE 40 // Korea
// static uint8_t g_sntp_server_ip[4] = {216, 239, 35, 8}; // time.google.com
time_t wizchip_sntp_get_current_timestamp(void)
{
    uint32_t retval = 0;
    uint32_t start_ms = 0;
    uint8_t loopcnt = 0;
    /* Get time */
    do {
        retval = SNTP_get_current_timestamp();
        sleep_ms(100);
        loopcnt++;
    } while (retval == 0 && loopcnt < 20);

    return (time_t)retval;
}
