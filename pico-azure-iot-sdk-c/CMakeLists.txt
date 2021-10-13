
# Set the project root directory if it's not already defined, as may happen if
# the tests folder is included directly by a parent project, without including
# the top level CMakeLists.txt.

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

set(main_files
application/main.c
application/iothub_ll_telemetry_sample.c
application/iothub_ll_c2d_sample.c
application/iothub_ll_client_x509_sample.c
application/prov_dev_client_ll_sample.c
application/custom_hsm_x509.c
)
#set(main_files ${iothub_c_files} ${PROJECT_SOURCE_DIR}/certs/certs.c)
add_executable(main ${main_files})

target_include_directories(main PUBLIC
.
${WIZNET_DIR}/Ethernet
${WIZNET_DIR}/Internet/dhcp
${WIZNET_DIR}/Internet/dns
${MBEDTLS_LIB_DIR}/include
${PORT_DIR}/inc
"${AZURE_SDK_DIR}/certs"
"${AZURE_SDK_DIR}/c-utility/inc"
"${AZURE_SDK_DIR}/c-utility/deps/azure-macro-utils-c/inc"
"${AZURE_SDK_DIR}/c-utility/deps/umock-c/inc"
"${AZURE_SDK_DIR}/c-utility/pal/inc"
"${AZURE_SDK_DIR}/c-utility/pal/freertos"
"${AZURE_SDK_DIR}/c-utility/pal/generic"
"${AZURE_SDK_DIR}/iothub_client/inc"
"${AZURE_SDK_DIR}/umqtt/inc"
"${AZURE_SDK_DIR}/umqtt/inc/azure_umqtt_c"
"${AZURE_SDK_DIR}/serializer/inc"
"${AZURE_SDK_DIR}/deps/parson"
)

set_source_files_properties(
    PROPERTIES COMPILE_FLAGS
    -Wno-maybe-uninitialized)

add_compile_options(
	-Wno-unused-function
	-Wno-missing-braces
	-Wno-missing-field-initializers
    )

add_subdirectory(${WIZNET_DIR})

set(ENABLE_PROGRAMS OFF CACHE BOOL "Build mbedtls programs")
set(ENABLE_TESTING OFF CACHE BOOL "Build mbedtls testing")
add_definitions(-DMBEDTLS_CONFIG_FILE="${PORT_DIR}/inc/mbedtls_config.h")
add_subdirectory(${MBEDTLS_LIB_DIR})
add_definitions(-DUSE_MQTT)
#add_definitions(-DUSE_HTTP)
add_subdirectory(${PORT_DIR})

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(main 
        pico_stdlib hardware_spi
        AzureSDK
        ETHERNET_FILES
        DHCP_FILES
        DNS_FILES
        mbedcrypto
        mbedx509
        mbedtls
)

# enable usb output, disable uart output
pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 0)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(main)

# add url via pico_set_program_url
example_auto_set_url(main)
