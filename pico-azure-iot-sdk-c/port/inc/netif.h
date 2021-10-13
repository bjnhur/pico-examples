// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>     
#include <stdbool.h>
#include <stdint.h>

uint8_t wizchip_gethostbyname(const char* host, uint8_t* ip);
void wizchip_sntp_init();
time_t wizchip_sntp_get_current_timestamp();
