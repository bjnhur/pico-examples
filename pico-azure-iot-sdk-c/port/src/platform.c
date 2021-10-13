// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdlib.h>
#ifdef _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>     
#include <time.h>

#include "azure_c_shared_utility/platform.h"
#include "azure_c_shared_utility/tickcounter.h"
#include "azure_c_shared_utility/tlsio_mbedtls.h"
#include "azure_c_shared_utility/xio.h"
#include "netif.h"

static const char* TAG = "platform";

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/

int platform_init(void)
{
	time_t now;
    struct tm timeinfo;
    wizchip_sntp_init();

    do {
		printf("Time is not set yet. Getting time over NTP. timeinfo.tm_year:%d\n",timeinfo.tm_year);
        now = wizchip_sntp_get_current_timestamp();
    	localtime_r(&now, &timeinfo);
	// Is time set? If not, tm_year will be (1970 - 1900).
    } while (timeinfo.tm_year < (2016 - 1900));
    printf("now - %lld\n", now);
    char strftime_buf[64];
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    printf("The current date/time is: %s\n", strftime_buf);

    // // maunal getting value from https://www.epochconverter.com/
    // time_t now = 1633303493;
    return 0;
}

const IO_INTERFACE_DESCRIPTION* platform_get_default_tlsio(void)
{
    // return tlsio_pal_get_interface_description();
    return tlsio_mbedtls_get_interface_description();
    // return NULL;
}

void platform_deinit(void)
{
    return;
}

STRING_HANDLE platform_get_platform_info(PLATFORM_INFO_OPTION options)
{
    // Expected format: "(<runtime name>; <operating system name>; <platform>)"
    return STRING_construct("(native; non-os; rp2040_W5100s platform)");
}
