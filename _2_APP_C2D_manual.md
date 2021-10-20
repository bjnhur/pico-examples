<!-- omit in toc -->
# iothub_ll_c2d_sample application
- [1. Prepare IoT Hub and Device for this example](#1-prepare-iot-hub-and-device-for-this-example)
  - [1.1. Azure portal](#11-azure-portal)
  - [1.2. Developer PC](#12-developer-pc)
    - [1.2.1. SDK setting](#121-sdk-setting)
    - [1.2.2. Build](#122-build)
    - [1.2.3. Azure IoT Explorer (preview) setting](#123-azure-iot-explorer-preview-setting)
    - [1.2.4. Serial terminal open](#124-serial-terminal-open)
- [2. Run the example](#2-run-the-example)
  - [2.1. copy main.uf2 file into your W5100S-EVB-Pico](#21-copy-mainuf2-file-into-your-w5100s-evb-pico)
  - [2.2. Send 3 messages from IoT device to IoT Hub](#22-send-3-messages-from-iot-device-to-iot-hub)
  - [2.3. Receive 3 C2D messages from IoT Hub to IoT device](#23-receive-3-c2d-messages-from-iot-hub-to-iot-device)
    - [2.3.1. How to send C2D message with Azure IoT Explorer](#231-how-to-send-c2d-message-with-azure-iot-explorer)
    - [2.3.2. Getting C2D messages](#232-getting-c2d-messages)


# 1. Prepare IoT Hub and Device for this example

## 1.1. Azure portal 
**[MUST]** Same as [`1.1. Azure portal`](_1_APP_TELEMETRY_manual.md#11-azure-portal) in  [`_1_APP_TELEMETRY_manual.md`](_1_APP_TELEMETRY_manual.md)


## 1.2. Developer PC

### 1.2.1. SDK setting

In the following [`pico-azure-iot-sdk-c/application/main.c`](pico-azure-iot-sdk-c/application/main.c) source file, find the line similar to this and replace it as you want:

```C
(...)

// The application you wish to use should be uncommented
//
//#define APP_TELEMETRY
#define APP_C2D
//#define APP_CLI_X509
//#define APP_PROV_X509

(...)

// The application you wish to use DHCP mode should be uncommented
#define _DHCP
static wiz_NetInfo g_net_info =
    {
        .mac = {0x00, 0x08, 0xDC, 0x12, 0x34, 0x11}, // MAC address
        .ip = {192, 168, 3, 111},                    // IP address
        .sn = {255, 255, 255, 0},                    // Subnet Mask
        .gw = {192, 168, 3, 1},                      // Gateway
        .dns = {8, 8, 8, 8},                         // DNS server
#ifdef _DHCP
        .dhcp = NETINFO_DHCP                         // DHCP enable/disable
#else
        // this example uses static IP
        .dhcp = NETINFO_STATIC
#endif
```

Edit the [`pico-azure-iot-sdk-c/application/sample_certs.c`](pico-azure-iot-sdk-c/application/sample_certs.c)  entering the proper connection string and key value from the Azure Portal:

```C

/* Paste in the your iothub connection string  */
const char pico_az_connectionString[] = "[device connection string]";

```

### 1.2.2. Build

Run `make` command

![image](https://user-images.githubusercontent.com/6334864/137408816-6f826c4e-e785-44e5-bc81-f35d04ce4d86.png)

### 1.2.3. Azure IoT Explorer (preview) setting

**[MUST]** Same as[1.2.3. Azure IoT Explorer (preview) setting](_1_APP_TELEMETRY_manual.md#123-azure-iot-explorer-preview-setting) in  [`_1_APP_TELEMETRY_manual.md`](_1_APP_TELEMETRY_manual.md)

### 1.2.4. Serial terminal open

Open "COM" port to see debug code

![image](https://user-images.githubusercontent.com/6334864/137317966-b9f63168-e011-4a0a-a3b1-345d1e847304.png)

# 2. Run the example 

This example has 2 functions:
1. Send 3 messages from IoT device.
2. Receive 3 C2D (Cloud to Device) messages.


## 2.1. copy main.uf2 file into your W5100S-EVB-Pico
![image](https://user-images.githubusercontent.com/6334864/137318763-14d23305-af22-45d1-ab43-4143b50b658c.png)

## 2.2. Send 3 messages from IoT device to IoT Hub 

Send 3 messages same as  [_1_APP_TELEMETRY_manual.md](_1_APP_TELEMETRY_manual.md)

You can get result of "function 1" result as below "Azure IoT Explorer" window.

![image](https://user-images.githubusercontent.com/6334864/137409633-fbc194ca-5f89-4517-85af-817e1a53e5db.png)
![image](https://user-images.githubusercontent.com/6334864/137409743-0de157bb-a37b-42ff-ae7e-f1a3e05188ca.png)

## 2.3. Receive 3 C2D messages from IoT Hub to IoT device
### 2.3.1. How to send C2D message with Azure IoT Explorer

For "function 2", you can send C2D messages to your device with "Azure IoT Explorer" program as follows:

![image](https://user-images.githubusercontent.com/6334864/137410000-726f4456-d81e-4154-9d86-cd3418d23f99.png)
![image](https://user-images.githubusercontent.com/6334864/137410174-51ea1f10-cc83-4187-aed4-44575ae90c11.png)

### 2.3.2. Getting C2D messages
Then, you can see the received #1 C2D message through your "Serial Terminal" window as below:

![image](https://user-images.githubusercontent.com/6334864/137410288-2f3a9f04-fab0-4c40-be9d-a502a76769f9.png)

- C2D Message #2

![image](https://user-images.githubusercontent.com/6334864/137410439-3fd284c9-87ef-4e42-8862-634e13363111.png)
![image](https://user-images.githubusercontent.com/6334864/137410450-3b02ac64-d754-418b-8d06-90ea390d2aa2.png)

- C2D Message #3

![image](https://user-images.githubusercontent.com/6334864/137410492-b12935bd-483a-452c-ae62-9cc287830023.png)
![image](https://user-images.githubusercontent.com/6334864/137410553-c7ef1408-e074-4dc8-b0db-6f1deb8b460a.png)
