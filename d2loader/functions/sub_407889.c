#include "sub_407889.h"
#include "sub_404ed0.h"

#define REPORT_VIRTUAL_PROTECT_ERROR \
do \
{ \
    sub_404ed0_LogFormat( \
        LOG_TAG, \
        "Virtual Protect Error %d", \
        GetLastError() \
    ); \
} while (FALSE) \

int sub_407889_smemcmp(
    const void* buffer1,
    const void* buffer2,
    size_t count
)
{
    if (buffer2 == NULL || buffer1 == NULL || count == 0)
    {
        return -1;
    }

    DWORD oldProtectFlagOfBuffer1;
    BOOL ret = VirtualProtect(
        (void*)buffer1,
        count,
        PAGE_EXECUTE_READWRITE,
        &oldProtectFlagOfBuffer1
    );
    if (!ret)
    {
        REPORT_VIRTUAL_PROTECT_ERROR;
        return -1;
    }

    DWORD oldProtectFlagOfBuffer2;
    ret = VirtualProtect(
        (void*)buffer2,
        count,
        PAGE_READWRITE,
        &oldProtectFlagOfBuffer2
    );
    DWORD unused;
    if (!ret)
    {
        REPORT_VIRTUAL_PROTECT_ERROR;
        ret = VirtualProtect(
            (void*)buffer1,
            count,
            oldProtectFlagOfBuffer1,
            &unused
        );
        if (!ret)
        {
            REPORT_VIRTUAL_PROTECT_ERROR;
        }
        return -1;
    }

    int compare = memcmp(buffer1, buffer2, count);

    ret = VirtualProtect(
        (void*)buffer2,
        count,
        oldProtectFlagOfBuffer2,
        &unused
    );
    if (!ret)
    {
        REPORT_VIRTUAL_PROTECT_ERROR;
    }

    ret = VirtualProtect(
        (void*)buffer1,
        count,
        oldProtectFlagOfBuffer1,
        &unused
    );
    if (!ret)
    {
        REPORT_VIRTUAL_PROTECT_ERROR;
    }

    return compare;
}