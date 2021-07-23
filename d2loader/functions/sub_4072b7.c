#include "pch.h"
#include "sub_4072b7.h"
#include <assert.h>
#include "../constants.h"
#include "../global-variables.h"
#include "sub_404ed0.h"
#include "sub_4061df.h"

BOOL sub_4072b7_ErrorHandle(
)
{
    sub_4061df_PluginListRun(RUN_PLUGIN_REASON_ERROR_CLEANUP);
    if (global_dd_408634_gameWindowHandle != NULL)
    {
        DestroyWindow(global_dd_408634_gameWindowHandle);
    }

    if (!global_dd_408620_settings->db_07a2_noNotify.value)
    {
        sub_404ed0_LogFormat(
            LOG_TAG,
            "Client Crashed"
        );
        MessageBoxA(NULL, "We got a big error here :(", "Hey guys", MB_ICONERROR);
    }

    if (global_dd_408620_settings->db_07a4_autoRestart.value)
    {
        sub_404ed0_LogFormat(
            LOG_TAG,
            "Try to Auto Restart..."
        );
        WinExec(GetCommandLineA(), SW_SHOW);
    }

    if (global_dd_408620_settings->db_07a3_noExit.value)
    {
        return TRUE;
    }

    sub_404ed0_LogFormat(
        LOG_TAG,
        "Exiting..."
    );
    ExitProcess(1);
    assert(ASSERT_UNREACHABLE_CODE);
}