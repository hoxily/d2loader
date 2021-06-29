#include <assert.h>
#include "sub_404c57.h"
#include "../global-variables.h"
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
            LOG_TAG(sub_404c57_GameMain),
            "Printing Parameters Table");
        sub_4066dc_PrintParametersTable(global_dd_408588_logFile);
        sub_404ed0_LogFormat(
            LOG_TAG(sub_404c57_GameMain),
            "Parameters Table Printed, Exitting");
    }
    else
    {
        if (global_dd_408620_settings->db_07ab_noPlugin.value)
        {
            sub_404ed0_LogFormat(
                LOG_TAG(sub_404c57_GameMain),
                "Plugin Have Been Disabled");
        }
        else
        {
            sub_406014_PluginInit();
        }

        BOOL hookRet = sub_4054fd_HookDll();
        sub_404ed0_LogFormat(
            LOG_TAG(sub_404c57_GameMain),
            "Hook Returned %s",
            hookRet ? "True" : "False");

        if (!sub_40a480())
        {
            sub_404ed0_LogFormat(
                LOG_TAG(sub_404c57_GameMain),
                "Game Initialize Failed, Exitting\n");
            return FALSE;
        }

        if (!global_dd_408620_settings->db_07aa_noScript.value)
        {
            sub_4057a8();
        }

        sub_404ed0_LogFormat(
            LOG_TAG(sub_404c57_GameMain),
            "Entering Game Client Loop");
        sub_404d77_GameClientLoop();
        sub_404ed0_LogFormat(
            LOG_TAG(sub_404c57_GameMain),
            "Exit From Game Client Loop, Cleanup");
        sub_40a600_Cleanup();
        // �����һ�� LogFormat ���ã�����û���ṩ�κβ���������֡�
        // call sub_404ed0
        // ����call sub_40a600_Cleanup ֮ǰ������LogFormat���ý�����û��ƽ��ջ��
        // ���������ٴε��� LogFormat���൱�������ϴ�push�Ĳ�����
        sub_404ed0_LogFormat(
            LOG_TAG(sub_404c57_GameMain),
            "Exit From Game Client Loop, Cleanup");
        // ʵ�ⷢ�֣���û��������� ��Exit From Game Client Loop, Cleanup��

        if (global_dd_408620_settings->db_07af_noCleanup.value)
        {
            ExitProcess(1);
            assert("Unreachable code.");
        }
        else
        {
            sub_4071a5();
            sub_405af1();
            sub_406175();
            sub_405602();
            sub_4069bc();
            sub_404ed0_LogFormat(
                LOG_TAG(sub_404c57_GameMain),
                "Cleanup Done, Exitting\n");
        }
    }

    return TRUE;

}