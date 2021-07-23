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

    // �ڻ����ʵ�α��� memorySize �����������ˡ��˴����¶���һ��������
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

    // ����е�4����������ʵ�� src ���������ˡ��˴��¶���һ���������������塣
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