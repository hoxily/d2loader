#include "pch.h"
#include "sub_406108.h"
#include "../global-variables.h"
#include "sub_4063f9.h"

BOOL sub_406108(
)
{
    if (global_dd_408610_plugins == NULL)
    {
        return FALSE;
    }

    EnterCriticalSection(&global_dd_4085f8_criticalSection);
    for (DWORD esi_i = 0; esi_i < global_dd_408614_count; esi_i++)
    {
        struct loaded_plugin_item* item = &global_dd_408610_plugins[esi_i];
        if (!item->dd_0000_activeFlag)
        {
            continue;
        }

        DWORD ret = WaitForSingleObject(
            item->dd_0008_autoResetEventHandle,
            0
        );
        if (ret != WAIT_OBJECT_0)
        {
            continue;
        }

        sub_4063f9_UnloadPlugin(item);
    }
    LeaveCriticalSection(&global_dd_4085f8_criticalSection);
    return TRUE;
}