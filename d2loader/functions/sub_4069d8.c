#include <string.h>
#include "sub_4069d8.h"
#include "../global-variables.h"
#include "../constant-strings.h"
#include "sub_406bab.h"
#include "sub_40735e.h"

void sub_4069d8_InitializeDefaultSettings()
{
    memset(global_dd_408620_settings, 0, sizeof(union program_setting_store));
    if (sub_40735e_CheckExpansion())
    {
        global_dd_408620_settings->db_0000_expansion.value = TRUE;
    }

    global_dd_408620_settings->dd_020d_IsExpansion.value = sub_406bab_IsExpansion;
    global_dd_408620_settings->db_079d_launch.value = TRUE;
    global_dd_408620_settings->db_07af_noCleanup.value = TRUE;
    strcpy(global_dd_408620_settings->db_0804_title.value, CSTR_D2_LOADER_VERSION_AND_BUILD);
    strcpy(global_dd_408620_settings->db_07ec_gameName.value, CSTR_DIABLO_II);
    // 怪不得以前打开windows的任务管理器查看d2loader.exe的优先级，总是显示为低于正常。
    strcpy(global_dd_408620_settings->db_07bc_processPriority.value, CSTR_PROCESS_PRIORITY_BELOW_NORMAL);
}
