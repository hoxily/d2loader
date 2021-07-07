#include "sub_4070d5.h"
#include "../global-variables.h"
#include "../constants.h"
#include "sub_4070aa.h"
#include "sub_404ed0.h"
#include "sub_405663.h"

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

    fn_D2_QueryInterface query = (fn_D2_QueryInterface)GetProcAddress(
        global_dd_408624_moduleBnClient,
        "QueryInterface"
    );
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
    
    // С���������� call �Ĳ��죺
    // call eax
    // call dword ptr [eax]
    // query ȡ�õ�ֻ��һ��ָ�룬ָ��һ������ָ�롣
    fn_D2_ComInt fn = *(fn_D2_ComInt*)query();

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