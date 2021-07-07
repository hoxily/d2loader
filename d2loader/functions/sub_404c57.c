#include <assert.h>
#include "sub_404c57.h"
#include "../global-variables.h"
#include "../constants.h"
#include "sub_404ed0.h"
#include "sub_4066dc.h"
#include "sub_406014.h"
#include "sub_4054fd.h"
#include "sub_40a480.h"
#include "sub_4057a8.h"
#include "sub_404d77.h"
#include "sub_40a600.h"
#include "sub_4071a5.h"
#include "sub_405af1.h"
#include "sub_406175.h"
#include "sub_405602.h"
#include "sub_4069bc.h"

BOOL sub_404c57_GameMain(
)
{
    if (global_dd_408620_settings->db_07a9_printArg.value)
    {
        sub_404ed0_LogFormat(
            LOG_TAG,
            "Printing Parameters Table");
        sub_4066dc_PrintParametersTable(global_dd_408588_logFile);
        sub_404ed0_LogFormat(
            LOG_TAG,
            "Parameters Table Printed, Exitting");
    }
    else
    {
        if (global_dd_408620_settings->db_07ab_noPlugin.value)
        {
            sub_404ed0_LogFormat(
                LOG_TAG,
                "Plugin Have Been Disabled");
        }
        else
        {
            sub_406014_PluginInit();
        }

        BOOL hookRet = sub_4054fd_HookDll();
        sub_404ed0_LogFormat(
            LOG_TAG,
            "Hook Returned %s",
            hookRet ? "True" : "False");

        if (!sub_40a480())
        {
            sub_404ed0_LogFormat(
                LOG_TAG,
                "Game Initialize Failed, Exitting\n");
            return FALSE;
        }

        if (!global_dd_408620_settings->db_07aa_noScript.value)
        {
            sub_4057a8_PatchByD2HackScript();
        }

        sub_404ed0_LogFormat(
            LOG_TAG,
            "Entering Game Client Loop");
        sub_404d77_GameClientLoop();
        sub_404ed0_LogFormat(
            LOG_TAG,
            "Exit From Game Client Loop, Cleanup");
        sub_40a600_Cleanup();
        // 这儿有一个 LogFormat 调用，但是没有提供任何参数，很奇怪。
        // call sub_404ed0
        // 由于call sub_40a600_Cleanup 之前的两次LogFormat调用结束后还没有平衡栈，
        // 所以这里再次调用 LogFormat，相当于沿用上次push的参数。
        sub_404ed0_LogFormat(
            LOG_TAG,
            "Exit From Game Client Loop, Cleanup");
        // 实测发现，并没有输出两遍 “Exit From Game Client Loop, Cleanup”

        if (global_dd_408620_settings->db_07af_noCleanup.value)
        {
            ExitProcess(1);
            assert(ASSERT_UNREACHABLE_CODE);
        }
        else
        {
            sub_4071a5();
            sub_405af1();
            sub_406175();
            sub_405602();
            sub_4069bc();
            sub_404ed0_LogFormat(
                LOG_TAG,
                "Cleanup Done, Exitting\n");
        }
    }

    return TRUE;

}