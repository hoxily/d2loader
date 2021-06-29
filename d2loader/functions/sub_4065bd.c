#include <assert.h>
#include "sub_4065bd.h"
#include "../global-variables.h"
#include "sub_404ed0.h"

BOOL sub_4065bd_AddPlugin(
    HMODULE dllModule,
    const char* dllFileName,
    struct query_interface_result* result
)
{
    CRITICAL_SECTION* edi_criticalSection = &global_dd_4085f8_criticalSection;
    EnterCriticalSection(edi_criticalSection);
    {
        if (global_dd_408614_count >= global_dd_40861c_capacity)
        {
            global_dd_40861c_capacity += 0xa;
            assert(sizeof(struct loaded_plugin_item) == 20);
            size_t newSize = global_dd_40861c_capacity * sizeof(struct loaded_plugin_item);
            // realloc 可能会返回NULL，Visual Studio要求检查返回值。
            void* ptr = realloc(global_dd_408610_plugins, newSize);
            assert(ptr != NULL);
            global_dd_408610_plugins = ptr;
        }

        assert(sizeof(struct loaded_plugin_item) == 20);
        struct loaded_plugin_item* item = &global_dd_408610_plugins[global_dd_408614_count];
        item->dd_0000_activeFlag = 1;
        item->dd_0004_dllModule = dllModule;
        item->dd_0008_autoResetEventHandle = CreateEventA(NULL, FALSE, FALSE, NULL);
        item->dd_000c_dllFileName = _strdup(dllFileName);
        item->dd_0010_queryInterfaceResult = result;

        global_dd_408618_activePluginCount++;
        global_dd_408614_count++;
    }
    LeaveCriticalSection(edi_criticalSection);

    sub_404ed0_LogFormat(
        LOG_TAG(sub_4065bd_AddPlugin),
        "Added Plugin %s: \"%s\"",
        dllFileName,
        result->pluginName);

    return TRUE;
}
