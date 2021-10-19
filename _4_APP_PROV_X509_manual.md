<!-- omit in toc -->
# prov_dev_client_ll_sample application

- [1. Prepare IoT Hub and Device for this example](#1-prepare-iot-hub-and-device-for-this-example)
  - [1.1. Developer PC - Generate Device self-signed certificates](#11-developer-pc---generate-device-self-signed-certificates)
  - [1.2. Azure portal](#12-azure-portal)
    - [1.2.1. Create Azure Device Provisioning service](#121-create-azure-device-provisioning-service)
    - [1.2.2. Link to Azure IoT Hub & DPS](#122-link-to-azure-iot-hub--dps)
    - [1.2.3. Create a device enrollment](#123-create-a-device-enrollment)
  - [1.3. Developer PC - SDK setting](#13-developer-pc---sdk-setting)
  - [1.4. Developer PC - Build](#14-developer-pc---build)
  - [1.5. Developer PC - Serial terminal open](#15-developer-pc---serial-terminal-open)
- [2. Run the example code](#2-run-the-example-code)
  - [2.1. copy main.uf2 file into your Pico board](#21-copy-mainuf2-file-into-your-pico-board)
  - [2.2. Azure IoT Explorer (preview) log](#22-azure-iot-explorer-preview-log)
  - [2.3. Serial terminal log](#23-serial-terminal-log)
  - [2.4. Azure IoT Explorer (preview) log](#24-azure-iot-explorer-preview-log)

---

# 1. Prepare IoT Hub and Device for this example
## 1.1. Developer PC - Generate Device self-signed certificates

**[MUST]** Please follow up [tutorial-x509-self-sign](https://docs.microsoft.com/en-us/azure/iot-hub/tutorial-x509-self-sign)

At last stage, you need to run the following command for making .pem file:
> openssl x509 -in prov_device1.crt -out prov_device1.pem -outform PEM

For your reference, prepare example log as below:
Notice! device ID = **"W5100S_EVB_PICO_PROV_X509"**

```bash

MINGW64 ~/certi
$ openssl genpkey -out prov_device1.key -algorithm RSA -pkeyopt rsa_keygen_bits:2048
............+++++
...................................+++++

MINGW64 ~/certi
$ openssl req -new -key prov_device1.key -out prov_device1.csr
You are about to be asked to enter information that will be incorporated
into your certificate request.
What you are about to enter is what is called a Distinguished Name or a DN.
There are quite a few fields but you can leave some blank
For some fields there will be a default value,
If you enter '.', the field will be left blank.
-----
Country Name (2 letter code) [AU]:.
State or Province Name (full name) [Some-State]:.
Locality Name (eg, city) []:.
Organization Name (eg, company) [Internet Widgits Pty Ltd]:.
Organizational Unit Name (eg, section) []:.
Common Name (e.g. server FQDN or YOUR name) []:W5100S_EVB_PICO_PROV_X509
Email Address []:

Please enter the following 'extra' attributes
to be sent with your certificate request
A challenge password []:
An optional company name []:

MINGW64 ~/certi
$ openssl x509 -req -days 365 -in prov_device1.csr -signkey prov_device1.key -out prov_device1.crt
Signature ok
subject=CN = W5100S_EVB_PICO_PROV_X509
Getting Private key

MINGW64 ~/certi
$ openssl x509 -in prov_device1.crt -out prov_device1.pem -outform PEM

MINGW64 ~/certi
$ ls
prov_device1.crt  prov_device1.key prov_device1.csr  prov_device1.pem

MINGW64 ~/certi
$

```

## 1.2. Azure portal 
### 1.2.1. Create Azure Device Provisioning service

**[MUST]** For Device Provisioning service creation, please follow up the 
[Quickstart: Set up the IoT Hub Device Provisioning Service with the Azure portal](https://docs.microsoft.com/en-us/azure/iot-dps/quick-setup-auto-provision) document first.

### 1.2.2. Link to Azure IoT Hub & DPS

Connect DPS and IoT Hub service

![image](https://user-images.githubusercontent.com/6334864/137447405-663588df-ba63-4951-8cac-7fbdd78bfd91.png)

### 1.2.3. Create a device enrollment

Add individual enrollment

![image](https://user-images.githubusercontent.com/6334864/137451545-a75c4293-3373-4414-9153-5b627c2f66c6.png)

Use "prov_device1.pem" file generated in previous section

![image](https://user-images.githubusercontent.com/6334864/137453279-4053bf2e-e0a7-4665-84b0-3232ddc4f231.png)

Check "Individual Enrollments" list

![image](https://user-images.githubusercontent.com/6334864/137453392-3e411a30-2c64-4cbb-bc95-2748c88efb30.png)

For more details, 

- please refer [How to manage device enrollments with Azure portal](https://docs.microsoft.com/en-us/azure/iot-dps/how-to-manage-enrollments)

- OR please read [Quickstart: Provision an X.509 certificate simulated device](https://docs.microsoft.com/en-us/azure/iot-dps/quick-create-simulated-device-x509?tabs=windows&pivots=programming-language-ansi-c) document as well.


## 1.3. Developer PC - SDK setting

- Get the key value from files _(prov_device1.crt, prov_device1.key)_ as below:

![image](https://user-images.githubusercontent.com/6334864/137454913-db68fbd4-b9ae-4c6f-8dd7-8eef3327851b.png)

- Edit [`/pico-azure-iot-sdk-c/application/sample_certs.c`](/pico-azure-iot-sdk-c/application/sample_certs.c) with generated certificates as upper. For Common name, Use "W5100S_EVB_PICO_PROV_X509" used in key generation.

![image](https://user-images.githubusercontent.com/6334864/137454964-aadef87a-e1f1-4835-ad9f-06eb0718b1f7.png)

- Select example in main.c 

In the following [`pico-azure-iot-sdk-c/application/main.c`](pico-azure-iot-sdk-c/application/main.c) source file, find the line similar to this and replace it as you want:

```C
(...)

// The application you wish to use should be uncommented
//
//#define APP_TELEMETRY
//#define APP_C2D
//#define APP_CLI_X509
#define APP_PROV

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

## 1.4. Developer PC - Build

Run `make` command

![image](https://user-images.githubusercontent.com/6334864/137421861-ada5ee2d-c153-4d75-bfb2-b8641f4d4919.png)

## 1.5. Developer PC - Serial terminal open

Open "COM" port to see debug code

![image](https://user-images.githubusercontent.com/6334864/137317966-b9f63168-e011-4a0a-a3b1-345d1e847304.png)


# 2. Run the example code

## 2.1. copy main.uf2 file into your Pico board

![image](https://user-images.githubusercontent.com/6334864/137318763-14d23305-af22-45d1-ab43-4143b50b658c.png)

## 2.2. Azure IoT Explorer (preview) log

Click "Refresh" until you find a provision device name

![image](https://user-images.githubusercontent.com/6334864/137456667-6ec35c58-5eda-4ee6-b5f1-ffbff394847b.png)

- After few seconds, you can find provison device from device list as belew:
  ![image](https://user-images.githubusercontent.com/6334864/137456757-dd48cdc4-aa4c-4f60-82b5-3a6b891d9b49.png)

Go to "Telemetry" menu, click "Start", and wait for incoming messages

![image](https://user-images.githubusercontent.com/6334864/137456837-69c489e4-b3c0-43bc-be56-1394e8413cc6.png)

## 2.3. Serial terminal log

Connect to Azure DPS (Device Provisioning Server)

![image](https://user-images.githubusercontent.com/6334864/137457261-1403a3d3-9c8f-4e5f-bce1-701580a34b8b.png)

Provision work is done

![image](https://user-images.githubusercontent.com/6334864/137457282-aeb84f7d-5b02-416f-ad12-bdf35c2c5913.png)

Send 2 messages to Azure IoT hub

![image](https://user-images.githubusercontent.com/6334864/137458064-4fb10693-89b6-49fd-ba17-78e2fabff233.png)

## 2.4. Azure IoT Explorer (preview) log

You can see 2 messages from device as below:

![image](https://user-images.githubusercontent.com/6334864/137457385-f0da06ae-541b-4431-a26c-5c3db5c9b37e.png)
