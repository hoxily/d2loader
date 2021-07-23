#include "pch.h"
#include "sub_407778.h"
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

void* sub_407778_smemcpy(
    void* dst,
    void* src,
    size_t memorySize
)
{
    if (src == NULL || dst == NULL || memorySize == 0)
    {
        return dst;
    }

    // 在汇编里实参变量 memorySize 被拿来复用了。此处重新定义一个变量。
    DWORD oldProtectOfDst;
    BOOL ret = VirtualProtect(
        dst,
        memorySize,
        PAGE_EXECUTE_READWRITE,
        &oldProtectOfDst
    );
    if (!ret)
    {
        REPORT_VIRTUAL_PROTECT_ERROR;
        return NULL;
    }

    DWORD oldProtectOfSrc;
    ret = VirtualProtect(
        src,
        memorySize,
        PAGE_READWRITE,
        &oldProtectOfSrc
    );

    // 汇编中第4个参数，把实参 src 拿来复用了。此处新定义一个变量，以免歧义。
    DWORD unused;
    if (!ret)
    {
        REPORT_VIRTUAL_PROTECT_ERROR;

        ret = VirtualProtect(
            dst,
            memorySize,
            oldProtectOfDst,
            &unused
        );
        if (!ret)
        {
            REPORT_VIRTUAL_PROTECT_ERROR;
        }
        return NULL;
    }

    void* var_8 = memcpy(dst, src, memorySize);

    ret = VirtualProtect(
        src,
        memorySize,
        oldProtectOfSrc,
        &unused
    );
    if (!ret)
    {
        REPORT_VIRTUAL_PROTECT_ERROR;
    }

    ret = VirtualProtect(
        dst,
        memorySize,
        oldProtectOfDst,
        &unused
    );

    if (!ret)
    {
        REPORT_VIRTUAL_PROTECT_ERROR;
    }

    return var_8;
}