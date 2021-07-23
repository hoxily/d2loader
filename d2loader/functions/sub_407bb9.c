#include "pch.h"
#include "sub_407bb9.h"
#include "../constant-strings.h"

BOOL sub_407bb9_InitializeProcessPriority(
    HANDLE hProcess,
    const char* priority
)
{
    // 汇编里出现 esp + 8 + hProcess, esp + 12 + hProcess, esp + 8 + priority，
    // 是因为这个函数不是bp-based栈帧，每次push一个东西，就会导致索引变化，相比bp-based要复杂一些。
    // IDA竟然分析出来了！

    if (!priority)
    {
        return FALSE;
    }

    if (!hProcess)
    {
        return FALSE;
    }

    DWORD priorityClass;
    if (_stricmp(priority, CSTR_PROCESS_PRIORITY_IDLE) == 0)
    {
        priorityClass = IDLE_PRIORITY_CLASS;
    }
    else if (_stricmp(priority, CSTR_PROCESS_PRIORITY_NORMAL) == 0)
    {
        priorityClass = NORMAL_PRIORITY_CLASS;
    }
    else if (_stricmp(priority, CSTR_PROCESS_PRIORITY_BELOW_NORMAL) == 0)
    {
        priorityClass = BELOW_NORMAL_PRIORITY_CLASS;
    }
    else if (_stricmp(priority, CSTR_PROCESS_PRIORITY_ABOVE_NORMAL) == 0)
    {
        priorityClass = ABOVE_NORMAL_PRIORITY_CLASS;
    }
    else if (_stricmp(priority, CSTR_PROCESS_PRIORITY_HIGH) == 0)
    {
        priorityClass = HIGH_PRIORITY_CLASS;
    }
    else if (_stricmp(priority, CSTR_PROCESS_PRIORITY_REALTIME) == 0)
    {
        priorityClass = REALTIME_PRIORITY_CLASS;
    }
    else
    {
        return FALSE;
    }

    return SetPriorityClass(hProcess, priorityClass);
}
