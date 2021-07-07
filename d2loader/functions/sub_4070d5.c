#include "sub_4070d5.h"
#include "../global-variables.h"
#include "../constants.h"
#include "sub_4070aa.h"
#include "sub_404ed0.h"
#include "sub_405663.h"

typedef void* (*fn_BnClient_QueryInterface)();
typedef void (__fastcall *fn_ComInt)(union program_setting_store* settings);

BOOL sub_4070d5_D2CommonInit(
)
{
    global_dd_408624_moduleBnClient = sub_4070aa_D2LoadLibrary("bnclient.dll");
    if (global_dd_408624_moduleBnClient == NULL)
    {
        return FALSE;
    }

    global_dd_408628_moduleD2Client = sub_4070aa_D2LoadLibrary("d2client.dll");
    if (global_dd_408628_moduleD2Client == NULL)
    {
        return FALSE;
    }

    global_dd_40862c_moduleD2Launch = sub_4070aa_D2LoadLibrary("d2launch.dll");
    if (global_dd_40862c_moduleD2Launch == NULL)
    {
        return FALSE;
    }

    if (!sub_405663_LoadDynamicFunctions())
    {
        sub_404ed0_LogFormat(
            LOG_TAG,
            "Error Loading Dynamic Functions"
        );
        return FALSE;
    }

    fn_BnClient_QueryInterface query = (fn_BnClient_QueryInterface)GetProcAddress(global_dd_408624_moduleBnClient, "QueryInterface");
    if (query == NULL)
    {
        sub_404ed0_LogFormat(
            LOG_TAG,
            "Error QueryInterface on bnclient.dll"
        );
        return FALSE;
    }

    void* comInt = query();
    global_dd_408620_settings->dd_0221_comInt.value = comInt;
    
    // 小心下面两种 call 的差异：
    // call eax
    // call dword ptr [eax]
    // query 取得的只是一个指针，指向一个函数指针。
    fn_ComInt fn = *(fn_ComInt*)query();

    fn(global_dd_408620_settings);

    if (global_dd_408620_settings->db_079b_client.value)
    {
        global_dd_408620_settings->dd_07b0_gameState.value = GAME_STATE_CLIENT;
    }
    else if (global_dd_408620_settings->db_079c_server.value)
    {
        global_dd_408620_settings->dd_07b0_gameState.value = GAME_STATE_SERVER;
    }
    else if (global_dd_408620_settings->db_079e_multi.value)
    {
        global_dd_408620_settings->dd_07b0_gameState.value = GAME_STATE_MULTI;
    }
    else
    {
        global_dd_408620_settings->dd_07b0_gameState.value = GAME_STATE_LAUNCH;
    }
    return TRUE;
}