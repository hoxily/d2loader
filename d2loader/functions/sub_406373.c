#include <assert.h>
#include "sub_406373.h"
#include "../global-variables.h"
#include "../constants.h"
#include "sub_404ed0.h"
#include "sub_4063f9.h"

BOOL sub_406373_RunPlugin(
    struct loaded_plugin_item* plugin,
    DWORD reasonFlag
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

    assert(offsetof(struct query_interface_result, runPlugin) == 0xc);
    fn_RunPlugin runPlugin = plugin->dd_0010_queryInterfaceResult->runPlugin;
    if (runPlugin == NULL)
    {
        return FALSE;
    }

    BOOL ret;
    if (reasonFlag == RUN_PLUGIN_REASON_INIT)
    {
        DWORD var_8_4[2];
        var_8_4[0] = global_dd_408620_settings->dd_07b4_gameProductVersionFlag.value;
        var_8_4[1] = (DWORD)plugin->dd_0008_autoResetEventHandle;
        ret = runPlugin(RUN_PLUGIN_REASON_INIT, &var_8_4[0]);
    }
    else
    {
        ret = runPlugin(reasonFlag, (DWORD*)&global_dd_408620_settings->db_0000_expansion.value);
    }

    if (reasonFlag == RUN_PLUGIN_REASON_CLEANUP)
    {
        return TRUE;
    }

    if (!ret)
    {
        sub_404ed0_LogFormat(
            LOG_TAG(sub_406373_RunPlugin),
            "Failed to Run Plugin %s (Reason: %d)",
            plugin->dd_000c_dllFileName,
            reasonFlag);

        sub_4063f9_UnloadPlugin(plugin);
    }

    return ret;
}
