#include "pch.h"
#include "sub_4069bc.h"
#include "../global-variables.h"

BOOL sub_4069bc_CleanupProgramSettingStore(
)
{
    if (global_dd_408620_settings != NULL)
    {
        free(global_dd_408620_settings);
        global_dd_408620_settings = NULL;
    }
    return TRUE;
}
