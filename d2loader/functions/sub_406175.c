#include "pch.h"
#include "sub_406175.h"
#include "../global-variables.h"
#include "sub_4063f9.h"

BOOL sub_406175_CleanupPlugins(
)
{
    if (global_dd_408610_plugins == NULL)
    {
        return FALSE;
    }

    EnterCriticalSection(&global_dd_4085f8_criticalSection);
    for (DWORD edi_i = 0; edi_i < global_dd_408614_count; edi_i++)
    {
        struct loaded_plugin_item* plugin = &global_dd_408610_plugins[edi_i];
        sub_4063f9_UnloadPlugin(plugin);
    }
    free(global_dd_408610_plugins);
    global_dd_408610_plugins = NULL;
    LeaveCriticalSection(&global_dd_4085f8_criticalSection);
    
    DeleteCriticalSection(&global_dd_4085f8_criticalSection);

    return TRUE;
}
