<!-- omit in toc -->
# Raspberry Pi Pico W5100S Azure IoT SDK Examples

RP2040 W5100 W5500 network examples - Azure cloud functions, Azure IoT SDK, Azure IoT device client, ...

- [1. 🎯 Azure-IoT-SDK](#1--azure-iot-sdk)
  - [1.1. 3rd party SDKs](#11-3rd-party-sdks)
- [2. 🎓 Getting started](#2--getting-started)
  - [2.1. 📚 Download SDK library](#21--download-sdk-library)
    - [2.1.1. Download azure-iot-sdk-c library](#211-download-azure-iot-sdk-c-library)
    - [2.1.2. Download mbedtls library](#212-download-mbedtls-library)
    - [2.1.3. Building mbedtls library](#213-building-mbedtls-library)
  - [2.2. 🗂 Build a sample](#22--build-a-sample)
    - [2.2.1. Modify CMakeLists.txt](#221-modify-cmakeliststxt)
    - [2.2.2. Set your board network information and select application](#222-set-your-board-network-information-and-select-application)
    - [2.2.3. Set the key information](#223-set-the-key-information)
  - [2.3. ⏳ Build project](#23--build-project)
    - [2.3.1. Build command](#231-build-command)
    - [2.3.2. Example command log](#232-example-command-log)
  - [2.4. 📝 Sample result](#24--sample-result)
    - [2.4.1. 📬 "iothub_ll_telemetry_sample" application result](#241--iothub_ll_telemetry_sample-application-result)
    - [2.4.2. 📩 "iothub_ll_c2d_sample" application result](#242--iothub_ll_c2d_sample-application-result)
    - [2.4.3. 🔐 "iothub_ll_client_x509_sample" application result](#243--iothub_ll_client_x509_sample-application-result)
    - [2.4.3. 🚢 "prov_dev_client_ll_sample" application result](#243--prov_dev_client_ll_sample-application-result)

------

# 1. 🎯 Azure-IoT-SDK

App|Description
---|---
[Azure IoT SDK example](pico-azure-iot-sdk-c) | Basic Azure cloud functions with Azure IoT SDK. (NonOS + WIZnet W5100S)

## 1.1. 3rd party SDKs
- [Azure IoT C SDKs and Libraries](https://github.com/Azure/azure-iot-sdk-c)
- [Mbed TLS](https://github.com/ARMmbed/mbedtls)
- [WIZnet ioLibrary](https://github.com/Wiznet/ioLibrary_Driver)
  - for CMake users => https://github.com/Wiznet/RP2040-HAT-C/tree/main/libraries/ioLibrary_Driver

# 2. 🎓 Getting started

See [Getting Started with the Raspberry Pi Pico](https://rptl.io/pico-get-started) and the README in the [pico-sdk](https://github.com/raspberrypi/pico-sdk) for information
on getting up and running.

## 2.1. 📚 Download SDK library 

### 2.1.1. Download azure-iot-sdk-c library
This repo get the local copy version for this project.

If you need, you can get this version from releae section in the git repo
- Recent release : https://github.com/Azure/azure-iot-sdk-c/releases/tag/LTS_07_2021_Ref01
- Azure IoT C SDK 1.4.1 - https://github.com/Azure/azure-iot-sdk-c/releases/tag/1.4.1

📑 For details, Please read [_0_install_Azure_IoT_SDK.md](_0_install_Azure_IoT_SDK.md)

For Pico W5100S platform, we need to make port codes, please check out the [Microsoft Azure SDK porting guide document](https://github.com/Azure/azure-c-shared-utility/blob/master/devdoc/porting_guide.md). From this porting guide, make `pico-azure-iot-sdk-c\port` directory.

### 2.1.2. Download mbedtls library
This repo get the local copy version for this project.

If you need, you can get this version from release section in their git repo
- Mbed TLS 3.0.0 - https://github.com/ARMmbed/mbedtls/releases/tag/v3.0.0

### 2.1.3. Building mbedtls library

For this project, (MUST) Need to change 2 files. - temporary fix
In the following mbedtls source file, find the line similar to this and replace it as follows:

1. \mbedtls-3.0.0\include\mbedtls\ssl.h

```C
// NOTICE! temporary solution for this project tlsio_mbedtls.c
//  int MBEDTLS_PRIVATE(out_msgtype); /*!< record header: message type */
  int out_msgtype;   /*!< record header: message type */

```

2. \mbedtls-3.0.0\include\mbedtls\x509_crt.h

```C
// NOTICE! temporary solution for this project tlsio_mbedtls.c
//  int MBEDTLS_PRIVATE(version); /**< The X.509 version. (1=v1, 2=v2, 3=v3) */
  int version;  /**< The X.509 version. (1=v1, 2=v2, 3=v3) */
```

## 2.2. 🗂 Build a sample

### 2.2.1. Modify CMakeLists.txt
In the following "CMakeLists.txt" file, find the line similar to this and replace it as your environment:

> pico-azure-iot-sdk-c/CMakeLists.txt
```bash

if(NOT DEFINED MBEDTLS_LIB_DIR)
#    set(MBEDTLS_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/mbedtls)
    set(MBEDTLS_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/mbedtls-3.0.0)
#    set(MBEDTLS_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/mbedtls-2.27.0)
    message(STATUS "MBEDTLS_LIB_DIR = ${MBEDTLS_LIB_DIR}")
endif()
if(NOT DEFINED AZURE_SDK_DIR)
    set(AZURE_SDK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/azure-iot-sdk-c-1.4.1)
    message(STATUS "AZURE_SDK_DIR = ${AZURE_SDK_DIR}")
endif()
if(NOT DEFINED PORT_DIR)
    set(PORT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/port)
    message(STATUS "PORT_DIR = ${PORT_DIR}")
endif()
if(NOT DEFINED WIZNET_DIR)
    set(WIZNET_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ioLibrary_Driver)
    message(STATUS "WIZNET_DIR = ${WIZNET_DIR}")
endif()
```

### 2.2.2. Set your board network information and select application

In the following main source file, find the line similar to this and replace it as follows:

> pico-azure-iot-sdk-c/application/main.c

```C
(...)

// The application you wish to use should be uncommented
//
#define APP_TELEMETRY
//#define APP_CLI_X509
//#define APP_C2D
//#define APP_PROV

(...)

static wiz_NetInfo g_net_info =
    {
        .mac = {0x00, 0x08, 0xDC, 0x12, 0x34, 0x11}, // MAC address
        .ip = {192, 168, 3, 111},                     // IP address
        .sn = {255, 255, 255, 0},                    // Subnet Mask
        .gw = {192, 168, 3, 1},                     // Gateway
        .dns = {8, 8, 8, 8},                         // DNS server

```

### 2.2.3. Set the key information

Edit the "sample_certs.c" file entering the proper connection string and key value from the Azure Portal:

> pico-azure-iot-sdk-c/application/sample_certs.c

```C
/* Paste in the your iothub connection string  */
const char pico_az_connectionString[] = "[device connection string]";
const char pico_az_x509_connectionString[] = "[device connection string]";
const char pico_az_x509certificate[] =
"-----BEGIN CERTIFICATE-----""\n"
"-----END CERTIFICATE-----";
const char pico_az_x509privatekey[] =
"-----BEGIN PRIVATE KEY-----""\n"
"-----END PRIVATE KEY-----";
const char pico_az_COMMON_NAME[] = "[custom-hsm-example]";
const char pico_az_CERTIFICATE[] = 
"-----BEGIN CERTIFICATE-----""\n"
"-----END CERTIFICATE-----";
const char pico_az_PRIVATE_KEY[] = 
"-----BEGIN PRIVATE KEY-----""\n"
"-----END PRIVATE KEY-----";
```

## 2.3. ⏳ Build project

### 2.3.1. Build command
Run the following CMake commands from the root of the repository:

```
mkdir build
cd build

# As your environment, select 
  cmake .. -G "MSYS Makefiles" ## on MSYS2 (MinGW64) + Windows 10 Platform
# OR
  cmake .. -G "Visual Studio 15 2017" ## For Visual Studio 2017
# or
  cmake .. -G "Visual Studio 16 2019" -A Win32
# or
  cmake ..
  
cd pico-azure-iot-sdk-c
make

```
Then, copy generated "main.uf2" file into your RP-Pico board. Done!!

### 2.3.2. Example command log

> on MSYS2 (MinGW64) + Windows 10 Platform

```
(PWD) pico-examples/build
$ cd ..

(PWD) pico-examples
$ rm -rf build

(PWD) pico-examples
$ mkdir build

(PWD) pico-examples
$ cd build

(PWD) pico-examples/build
$ cmake .. -G "MSYS Makefiles"
-- PICO_SDK_PATH = /../../RP2040/pico-sdk
PICO_SDK_PATH is pico-sdk
Defaulting PICO_PLATFORM to rp2040 since not specified.
Defaulting PICO platform compiler to pico_arm_gcc since not specified.
-- Defaulting build type to 'Release' since not specified.
PICO compiler is pico_arm_gcc
-- The C compiler identification is GNU 10.1.0
-- The CXX compiler identification is GNU 10.1.0
-- The ASM compiler identification is GNU
-- Found assembler: C:/msys64/mingw64/bin/arm-none-eabi-gcc.exe
Defaulting PICO target board to pico since not specified.
Using board configuration from pico-sdk/src/boards/include/boards/pico.h
-- Found Python3: C:/Users/User1/AppData/Local/Programs/Python/Python39/python.exe (found version "3.9.7") found components: Interpreter
TinyUSB available at pico-sdk/lib/tinyusb/src/portable/raspberrypi/rp2040; adding USB support.
-- MBEDTLS_LIB_DIR = pico-examples/pico-azure-iot-sdk-c/mbedtls-3.0.0
-- AZURE_SDK_DIR = pico-examples/pico-azure-iot-sdk-c/azure-iot-sdk-c-1.4.1
-- PORT_DIR = pico-examples/pico-azure-iot-sdk-c/port
-- WIZNET_DIR = pico-examples/pico-azure-iot-sdk-c/ioLibrary_Driver
-- Performing Test C_COMPILER_SUPPORTS_WFORMAT_SIGNEDNESS
-- Performing Test C_COMPILER_SUPPORTS_WFORMAT_SIGNEDNESS - Failed
-- Configuring done
-- Generating done
-- Build files have been written to: pico-examples/build

(PWD) pico-examples/build
$ cd pico-azure-iot-sdk-c/

(PWD) pico-examples/build/pico-azure-iot-sdk-c
$ make
[  0%] Creating directories for 'ELF2UF2Build'
[  1%] No download step for 'ELF2UF2Build'
[  1%] No update step for 'ELF2UF2Build'
[  1%] No patch step for 'ELF2UF2Build'
[  1%] Performing configure step for 'ELF2UF2Build'
-- The C compiler identification is GNU 10.3.0
-- The CXX compiler identification is GNU 10.3.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: C:/msys64/mingw64/bin/gcc.exe - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: C:/msys64/mingw64/bin/g++.exe - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: pico-examples/build/elf2uf2
[  1%] Performing build step for 'ELF2UF2Build'
[ 50%] Building CXX object CMakeFiles/elf2uf2.dir/main.cpp.obj
[100%] Linking CXX executable elf2uf2.exe
[100%] Built target elf2uf2
[  1%] No install step for 'ELF2UF2Build'
[  1%] Completed 'ELF2UF2Build'
[  1%] Built target ELF2UF2Build
Scanning dependencies of target bs2_default
[  1%] Building ASM object pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default.dir/compile_time_choice.S.obj
[  1%] Linking ASM executable bs2_default.elf
[  1%] Built target bs2_default
[  1%] Generating bs2_default.bin
[  2%] Generating bs2_default_padded_checksummed.S
[  2%] Built target bs2_default_padded_checksummed_asm
Scanning dependencies of target W5100S_FILES
[  3%] Building C object pico-azure-iot-sdk-c/ioLibrary_Driver/Ethernet/W5100S/CMakeFiles/W5100S_FILES.dir/w5100s.c.obj
[  3%] Building C object pico-azure-iot-sdk-c/ioLibrary_Driver/Ethernet/W5100S/CMakeFiles/W5100S_FILES.dir/D_/RP2040/pico-sdk/src/rp2_common/pico_stdlib/stdlib.c.obj

(...)

[100%] Building C object pico-azure-iot-sdk-c/CMakeFiles/main.dir/D_/RP2040/pico-sdk/src/rp2_common/hardware_flash/flash.c.obj
[100%] Building C object pico-azure-iot-sdk-c/CMakeFiles/main.dir/D_/RP2040/pico-sdk/src/rp2_common/hardware_spi/spi.c.obj
[100%] Linking CXX executable main.elf
[100%] Built target main

(PWD) pico-examples/build/pico-azure-iot-sdk-c
$ cp main.uf2 /f/

```

## 2.4. 📝 Sample result
### 2.4.1. 📬 "iothub_ll_telemetry_sample" application result

📑 [Let's see this doc for iothub_ll_telemetry_sample application](_1_APP_TELEMETRY_manual.md)

### 2.4.2. 📩 "iothub_ll_c2d_sample" application result

📑 [Let's see this doc for iothub_ll_c2d_sample application](_2_APP_C2D_manual.md)

### 2.4.3. 🔐 "iothub_ll_client_x509_sample" application result

📑 [Let's see this doc for iothub_ll_client_x509_sample application](_3_APP_CLIENT_X509_manual.md)

### 2.4.3. 🚢 "prov_dev_client_ll_sample" application result

📑 [Let's see this doc for prov_dev_client_ll_sample application](_4_APP_PROV_X509_manual.md)
