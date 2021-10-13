// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
#include "pico/stdlib.h"
#include "azure_c_shared_utility/xlogging.h"
#include "azure_c_shared_utility/threadapi.h"

void ThreadAPI_Sleep(unsigned int milliseconds)
{
    sleep_ms(milliseconds);
}

THREADAPI_RESULT ThreadAPI_Create(THREAD_HANDLE* threadHandle, THREAD_START_FUNC func, void* arg)
{
    (void)threadHandle;
    (void)func;
    (void)arg;
    printf("NonOS does not support threading.");
    return THREADAPI_ERROR;
}

THREADAPI_RESULT ThreadAPI_Join(THREAD_HANDLE threadHandle, int* res)
{
    (void)threadHandle;
    (void)res;
    printf("NonOS does not support threading.");
    return THREADAPI_ERROR;
}

void ThreadAPI_Exit(int res)
{
    (void)res;
    printf("NonOS does not support threading.");
}
