#include "pch.h"
#include "sub_4063f9.h"
#include "../global-variables.h"
#include "../constants.h"
#include "sub_404ed0.h"
#include "sub_406373.h"

BOOL sub_4063f9_UnloadPlugin(
    struct loaded_plugin_item* plugin
)
{
    if (plugin == NULL)
    {
        return FALSE;
    }

    if (plugin->dd_0000_activeFlag == 0)
    {
        return FALSE;
    }

    sub_404ed0_LogFormat(
        LOG_TAG,
        "Unloading Plugin %s",
        plugin->dd_000c_dllFileName);

    sub_406373_RunPlugin(plugin, RUN_PLUGIN_REASON_CLEANUP);
    plugin->dd_0000_activeFlag = 0;
    free(plugin->dd_000c_dllFileName);
    CloseHandle(plugin->dd_0008_autoResetEventHandle);
    FreeLibrary(plugin->dd_0004_dllModule);
    global_dd_408618_activePluginCount--;

    return TRUE;
}
