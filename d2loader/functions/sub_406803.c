#include <assert.h>
#include "sub_406803.h"
#include "../global-variables.h"
#include "sub_4069d8.h"
#include "sub_406887.h"
#include "sub_4068f2.h"
#include "sub_406bb9.h"
#include "sub_407bb9.h"

BOOL sub_406803_InitializeSettings()
{
    assert(sizeof(union program_setting_store) == 0xc94);
    global_dd_408620_settings = malloc(sizeof(union program_setting_store));
    if (!global_dd_408620_settings)
    {
        return FALSE;
    }
    sub_4069d8_InitializeDefaultSettings();
    char* commandLine = GetCommandLineA();
    if (!sub_406887_ProcessCommandLineArguments(commandLine))
    {
        return FALSE;
    }
    sub_4068f2_LoadConfFile(global_dd_408620_settings->db_0884_confFile.value);

    BYTE* videoModeFlags = &global_dd_408620_settings->db_0004_window.value;
    if (videoModeFlags[0] == 0 &&
        videoModeFlags[1] == 0 &&
        videoModeFlags[2] == 0 &&
        videoModeFlags[3] == 0 &&
        videoModeFlags[4] == 0)
    {
        sub_406bb9_LoadVideoConfigFromRegistry();
    }

    char* priority = global_dd_408620_settings->db_07bc_processPriority.value;
    // GetCurrentProcess 返回的是一个伪Handle，总是 (HANDLE)-1 即 0xffffffff。
    // 这并非出错了，而是故意这样设计的。
    HANDLE hProcess = GetCurrentProcess();
    sub_407bb9_InitializeProcessPriority(hProcess, priority);

    return TRUE;
}
