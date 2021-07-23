#include "pch.h"
#include "sub_4073ac.h"
#include "../global-variables.h"
#include <assert.h>
#include <stdio.h>
#include "sub_404ed0.h"

BOOL sub_4073ac_D2RunMultiClient(
)
{
    char commandLine[0x104];
    char tempPath[0x104];
    char tempFileName[0x104];
    STARTUPINFOA startupInfo;
    PROCESS_INFORMATION processInfo;
    assert(sizeof(STARTUPINFOA) == 0x54 - 0x10);
    assert(sizeof(PROCESS_INFORMATION) == 0x10);

    /*
    * 汇编中是使用 rep stosd 指令来初始化 startupInfo 结构体。如下所示：
    * push 10h
    * pop ecx
    * xor eax, eax,
    * lea edi, [ebp + startupInfo.lpReserved]
    * mov [ebp + startupInfo.cb], 44h
    * rep stosd
    */
    assert(sizeof(STARTUPINFOA) == 0x44);
    memset(&startupInfo.lpReserved, 0, sizeof(STARTUPINFOA));
    startupInfo.cb = sizeof(STARTUPINFOA);

    GetTempPathA(sizeof(tempPath), tempPath);
    GetTempFileNameA(tempPath, "d2l", 0, tempFileName);
    FILE* confFile = fopen(tempFileName, "w");
    if (!confFile)
    {
        sub_404ed0_LogFormat(
            LOG_TAG,
            "Failed to open Temp File \"%s\"",
            tempFileName
        );
        return FALSE;
    }

    union program_setting_store* pss = global_dd_408620_settings;
    fprintf(
        confFile,
        "-rmconffile -client -clientexit -charclass %d -charstatus %d -gametype %d -name \"%s\" -gamename \"%s\" -gamepass \"%s\" -s \"%s\" -joinid %d -token %d -title \"%s(*%s) %s@%s\"",
        pss->db_01e9_characterClass.value,
        pss->dw_01ea_characterStatus.value,
        3,
        pss->db_00b8_name.value,
        pss->db_001a_gameName.value,
        pss->db_023f_gamePass.value,
        pss->db_0032_serverIp.value,
        pss->dw_0018_joinId.value,
        pss->dd_0223_token.value,
        pss->db_00b8_name.value,
        pss->db_0088_bnAccount.value,
        pss->db_001a_gameName.value,
        pss->db_00d0_realm.value
    );

    if (pss->db_0080_ama.value)
    {
        fprintf(confFile, " -ama");
    }
    if (pss->db_0083_nec.value)
    {
        fprintf(confFile, " -nec");
    }
    if (pss->db_0082_sor.value)
    {
        fprintf(confFile, " -sor");
    }
    if (pss->db_0084_bar.value)
    {
        fprintf(confFile, " -bar");
    }
    if (pss->db_0081_pal.value)
    {
        fprintf(confFile, " -pal");
    }
    if (pss->db_0085_dru.value)
    {
        fprintf(confFile, " -dru");
    }
    if (pss->db_0086_asn.value)
    {
        fprintf(confFile, " -asn");
    }
    fclose(confFile);

    // 这样子生成的commandLine是有问题的。对于那些无参数值的参数，其实是在做TRUE/FALSE切换操作。
    // 两个 -ama 等价于保留默认的值。
    // 所以应该去除 GetCommandLineA 结果里的参数，只保留程序部分。
    wsprintfA(
        commandLine,
        "%s -conffile \"%s\"",
        GetCommandLineA(),
        tempFileName
    );

    BOOL ret = CreateProcessA(
        NULL, // applicationName
        commandLine,
        NULL, // processAttributes
        NULL, // threadAttributes
        FALSE, // inheritHandles
        NORMAL_PRIORITY_CLASS, // creationFlags
        NULL, // environment
        NULL, // currentDirectory
        &startupInfo,
        &processInfo
    );

    if (!ret)
    {
        sub_404ed0_LogFormat(
            LOG_TAG,
            "Error Spawning Child (Error: %d)",
            GetLastError()
        );
        return FALSE;
    }

    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);

    if (global_dd_408634_gameWindowHandle != NULL)
    {
        ShowWindow(global_dd_408634_gameWindowHandle, SW_MINIMIZE);
    }
    SetPriorityClass(
        GetCurrentProcess(),
        IDLE_PRIORITY_CLASS
    );
    return TRUE;
}