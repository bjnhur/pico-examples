// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "azure_c_shared_utility/lock.h"
#include "azure_c_shared_utility/xlogging.h"

LOCK_HANDLE Lock_Init(void)
{
    printf("No support lock functions.\n");
    return (LOCK_HANDLE)NULL;
}

LOCK_RESULT Lock_Deinit(LOCK_HANDLE handle)
{
    printf("No support lock functions; handle is NULL.\n");
    return LOCK_ERROR;
}

LOCK_RESULT Lock(LOCK_HANDLE handle)
{
    printf("No support lock functions; handle is NULL.\n");
    return LOCK_ERROR;
}

LOCK_RESULT Unlock(LOCK_HANDLE handle)
{
    printf("No support lock functions; handle is NULL.\n");
    return LOCK_ERROR;
}
