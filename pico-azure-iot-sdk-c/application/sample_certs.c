#include "azure_samples.h"

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
"BASE64 Encoded certificate Here""\n"
"-----END CERTIFICATE-----";

const char pico_az_PRIVATE_KEY[] = 
"-----BEGIN PRIVATE KEY-----""\n"
"BASE64 Encoded certificate Here""\n"
"-----END PRIVATE KEY-----";
