# Raspberry Pi Pico W5100 W5500 Network SDK Examples

RP2040 W5100 W5500 network examples - Azure cloud functions, Azure IoT SDK, Azure IoT device client, ...

## Azure-IoT-SDK

App|Description
---|---
[Azure IoT SDK example](pico-azure-iot-sdk-c) | Basic Azure cloud functions with Azure IoT SDK. (NonOS + WIZnet W5100S)

### 3rd party SDKs
- [Azure IoT C SDKs and Libraries](https://github.com/Azure/azure-iot-sdk-c)
- [Mbed TLS](https://github.com/ARMmbed/mbedtls)
- [WIZnet ioLibrary](https://github.com/Wiznet/ioLibrary_Driver)
  - for CMake users => https://github.com/Wiznet/RP2040-HAT-C/tree/main/libraries/ioLibrary_Driver
## Getting started

See [Getting Started with the Raspberry Pi Pico](https://rptl.io/pico-get-started) and the README in the [pico-sdk](https://github.com/raspberrypi/pico-sdk) for information
on getting up and running.

### Download SDK library 

#### Download azure-iot-sdk-c library
This repo get the local copy version for this project.

If you need, you can get this version from releae section in the git repo
- Azure IoT C SDK 1.4.1 - https://github.com/Azure/azure-iot-sdk-c/releases/tag/1.4.1

For Pico W5100S platform, we need to make port codes, please check out the [Microsoft Azure SDK porting guide document](https://github.com/Azure/azure-c-shared-utility/blob/master/devdoc/porting_guide.md). From this porting guide, make `\port` directory.

#### Download mbedtls library
This repo get the local copy version for this project.

If you need, you can get this version from release section in their git repo
- Mbed TLS 3.0.0 - https://github.com/ARMmbed/mbedtls/releases/tag/v3.0.0

(MUST) Need to change 2 files. - temporary fix

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

-------------
### Build project

1. set the SDK, library path in CMakeLists.txt
2. set the key string at application/sample_certs.c
3. mkdir build
4. cd build
5. cmake .. -G "MSYS Makefiles"
6. cd pico-azure-iot-sdk-c
7. make
8. cp main.uf2 into your RP-Pico board

#### Example command log


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

### Sample result

#### "iothub_ll_telemetry_sample" application result

> Screenshot of terminal window

![image](https://user-images.githubusercontent.com/6334864/137256887-c3387b1f-19a7-4006-bb84-caeefd847b67.png)

> Screenshot of "Azure IoT Explorer"

![image](https://user-images.githubusercontent.com/6334864/137255335-1d866b48-c26f-4ac9-a6e4-1015f1e71fc5.png)


-------
## Git tip!

### remove submodule

```
git submodule deinit <your_submodule> 
git rm <your_submodule> 
git commit-m "Removed submodule" 
rm -rf .git/modules/<your_submodule>

```
### git update all submodule

```
git submodule foreach 'git pull'

```
