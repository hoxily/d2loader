#include "sub_407778.h"
#include "sub_404ed0.h"

void* sub_407778_smemcpy(
    void* memory,
    void* arg1,
    size_t memorySize
)
{
    if (arg1 == NULL)
    {
        return memory;
    }

    if (memory == NULL)
    {
        return memory;
    }

    if (memorySize == 0)
    {
        return memory;
    }

    // �ڻ����ʵ�α��� memorySize �����������ˡ��˴����¶���һ��������
    DWORD oldProtectOfMemory;
    BOOL ret = VirtualProtect(
        memory,
        memorySize,
        PAGE_EXECUTE_READWRITE,
        &oldProtectOfMemory
    );
    if (!ret)
    {
        sub_404ed0_LogFormat(
            LOG_TAG,
            "Virtual Protect Error %d",
            GetLastError()
        );
        return NULL;
    }

    DWORD oldProtectOfArg1;
    ret = VirtualProtect(
        arg1,
        memorySize,
        PAGE_READWRITE,
        &oldProtectOfArg1
    );

    // ����е�4����������ʵ�� arg1 ���������ˡ��˴��¶���һ���������������塣
    DWORD unused;
    if (!ret)
    {
        sub_404ed0_LogFormat(
            LOG_TAG,
            "Virtual Protect Error %d",
            GetLastError()
        );

        
        ret = VirtualProtect(
            memory,
            memorySize,
            oldProtectOfMemory,
            &unused
        );
        if (!ret)
        {
            sub_404ed0_LogFormat(
                LOG_TAG,
                "Virtual Protect Error %d",
                GetLastError()
            );
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
        sub_404ed0_LogFormat(
            LOG_TAG,
            "Virtual Protect Error %d",
            GetLastError()
        );
    }

    ret = VirtualProtect(
        memory,
        memorySize,
        oldProtectOfMemory,
        &unused
    );

    if (!ret)
    {
        sub_404ed0_LogFormat(
            LOG_TAG,
            "Virtual Protect Error %d",
            GetLastError()
        );
        
    }

    return var_8;
}