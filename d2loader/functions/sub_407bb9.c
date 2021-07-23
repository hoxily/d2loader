#include "pch.h"
#include "sub_407bb9.h"
#include "../constant-strings.h"

BOOL sub_407bb9_InitializeProcessPriority(
    HANDLE hProcess,
    const char* priority
)
{
    // �������� esp + 8 + hProcess, esp + 12 + hProcess, esp + 8 + priority��
    // ����Ϊ�����������bp-basedջ֡��ÿ��pushһ���������ͻᵼ�������仯�����bp-basedҪ����һЩ��
    // IDA��Ȼ���������ˣ�

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
