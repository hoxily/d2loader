#include "sub_4061df.h"
#include "../global-variables.h"
#include "../constants.h"
#include "sub_40a4e5.h"
#include "sub_404ed0.h"
#include "sub_406373.h"

int sub_4061df_PluginListRun(
    DWORD reasonFlag
)
{
    if (global_dd_408610_plugins == NULL)
    {
        if (reasonFlag & RUN_PLUGIN_REASON_ENTER_MAIN_MENU)
        {
            sub_40a4e5();
        }

        return 0;
    }

    char reasonString[0x100];
    reasonString[0] = '\0';
    if (reasonFlag & RUN_PLUGIN_REASON_INIT)
    {
        strcat(reasonString, "REASON_INIT |");
    }
    if (reasonFlag & RUN_PLUGIN_REASON_LEAVE_GAME)
    {
        strcat(reasonString, "REASON_LEAVE_GAME |");
    }
    if (reasonFlag & RUN_PLUGIN_REASON_LEAVE_CHANNEL)
    {
        strcat(reasonString, "REASON_LEAVE_CHANNEL |");
    }
    if (reasonFlag & RUN_PLUGIN_REASON_LEAVE_MAIN_MENU)
    {
        strcat(reasonString, "REASON_LEAVE_MAIN_MENU |");
    }
    if (reasonFlag & RUN_PLUGIN_REASON_ENTER_MAIN_MENU)
    {
        sub_40a4e5();
        strcat(reasonString, "REASON_ENTER_MAIN_MENU |");
    }
    if (reasonFlag & RUN_PLUGIN_REASON_ENTER_CHANNEL)
    {
        strcat(reasonString, "REASON_ENTER_CHANNEL |");
    }
    if (reasonFlag & RUN_PLUGIN_REASON_CLEANUP)
    {
        strcat(reasonString, "REASON_CLEANUP |");
    }
    if (reasonFlag & RUN_PLUGIN_REASON_ENTER_GAME)
    {
        strcat(reasonString, "REASON_ENTER_GAME |");
    }
    // ���涼�� test bl, ������
    // �������� test bh, 1
    // ������������ĵ�8λΪ��
    if (reasonFlag & RUN_PLUGIN_REASON_ERROR_CLEANUP)
    {
        strcat(reasonString, "REASON_ERROR_CLEANUP |");
    }

    if (reasonString[0] == '\0')
    {
        // edi ��ĿǰΪֹ���� 0�����strcpy����û�����塣
        // ��archlinux���������Ĳ����ᵼ�¶δ��󡣿�������δ�������ǿ�ƽ������̣�
        //strcpy(reasonString, NULL);

        // fix by hoxily@qq.com
        strcpy(reasonString, "");
    }
    else
    {
        size_t len = strlen(reasonString);
        // ����о�Ӧ��Ҫ len - 2 ���ܰ�ĩβ�Ŀո�����߳�ȥ��
        // fix by hoxily@qq.com
        reasonString[len - 2] = '\0';
    }

    sub_404ed0_LogFormat(
        LOG_TAG(sub_4061df_PluginListRun),
        "Call All Plugins With %s",
        reasonString);

    int runCount = 0;
    EnterCriticalSection(&global_dd_4085f8_criticalSection);
    {
        // fix by hoxily@qq.com: ����Ӧ���� global_dd_408614_count ��
        // ��Ϊ�� RunPlugin �����ڲ����п��ܶ�plugin��ж�ز���������activePluginCountֵ��
        for (DWORD ebx_i = 0; ebx_i < global_dd_408614_count; ebx_i++)
        {
            BOOL ret = sub_406373_RunPlugin(&global_dd_408610_plugins[ebx_i], reasonFlag);
            if (ret)
            {
                runCount++;
            }
        }
    }
    LeaveCriticalSection(&global_dd_4085f8_criticalSection);

    return runCount;
}
