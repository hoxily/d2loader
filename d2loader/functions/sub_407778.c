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
    void* memory,
    void* arg1,
    size_t memorySize
)
{
    if (arg1 == NULL || memory == NULL || memorySize == 0)
    {
        return memory;
    }

    // 在汇编里实参变量 memorySize 被拿来复用了。此处重新定义一个变量。
    DWORD oldProtectOfMemory;
    BOOL ret = VirtualProtect(
        memory,
        memorySize,
        PAGE_EXECUTE_READWRITE,
        &oldProtectOfMemory
    );
    if (!ret)
    {
        REPORT_VIRTUAL_PROTECT_ERROR;
        return NULL;
    }

    DWORD oldProtectOfArg1;
    ret = VirtualProtect(
        arg1,
        memorySize,
        PAGE_READWRITE,
        &oldProtectOfArg1
    );

    // 汇编中第4个参数，把实参 arg1 拿来复用了。此处新定义一个变量，以免歧义。
    DWORD unused;
    if (!ret)
    {
        REPORT_VIRTUAL_PROTECT_ERROR;

        ret = VirtualProtect(
            memory,
            memorySize,
            oldProtectOfMemory,
            &unused
        );
        if (!ret)
        {
            REPORT_VIRTUAL_PROTECT_ERROR;
        }
        return NULL;
    }

    void* var_8 = memcpy(memory, arg1, memorySize);

    ret = VirtualProtect(
        arg1,
        memorySize,
        oldProtectOfArg1,
        &unused
    );
    if (!ret)
    {
        REPORT_VIRTUAL_PROTECT_ERROR;
    }

    ret = VirtualProtect(
        memory,
        memorySize,
        oldProtectOfMemory,
        &unused
    );

    if (!ret)
    {
        REPORT_VIRTUAL_PROTECT_ERROR;
    }

    return var_8;
}