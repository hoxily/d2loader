#include <stdio.h>
#include "sub_404b60.h"
#include "../global-variables.h"
#include "../constant-strings.h"
#include "../constants.h"
#include "sub_406803.h"
#include "sub_404eb1.h"
#include "sub_404ec5.h"
#include "sub_406c59.h"
#include "sub_404ed0.h"
#include "sub_404c57.h"

int WINAPI sub_404b60_WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd
)
{
    if (!sub_406803_InitializeSettings())
    {
        return 0;
    }

    if (global_dd_408620_settings->db_07ac_enableLogFile.value)
    {
        global_dd_408588_logFile = fopen(CSTR_D2_LOADER_DOT_LOG, "a");
        sub_404eb1_SetLogFile(global_dd_408588_logFile);
        sub_404ec5_EnableLog(LOG_TYPE_FILE);
        // add esp, 10h 是平衡前面的三个C函数调用造成的栈变化
    }

    sub_406c59_CheckGameExeVersion();
    sub_404ed0_LogFormat(LOG_TAG, CSTR_LOG_START_SEPARATOR_LINE);
    sub_404ed0_LogFormat(LOG_TAG, CSTR_D2_LOADER_AUTHOR_STATEMENT);
    char* commandLine = GetCommandLineA();
    sub_404ed0_LogFormat(LOG_TAG, "Command Line: %s", commandLine);
    sub_404ed0_LogFormat(LOG_TAG, "Loader Version: %s", CSTR_D2_LOADER_VERSION_AND_BUILD);
    sub_404ed0_LogFormat(LOG_TAG, "Client Version: 0x%08X", global_dd_408620_settings->dd_07b4_gameProductVersionFlag.value);
    const char* mode = global_dd_408620_settings->db_0000_expansion.value ? "Expansion" : "Classic";
    sub_404ed0_LogFormat(LOG_TAG, "Running in %s Mode", mode);

    sub_404c57_GameMain();

    sub_404eb1_SetLogFile(NULL);

    if (global_dd_408588_logFile)
    {
        fclose(global_dd_408588_logFile);
    }

    return 1;
}
