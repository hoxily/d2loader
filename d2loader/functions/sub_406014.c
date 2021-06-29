#include <assert.h>
#include "sub_406014.h"
#include "../global-variables.h"
#include "../constants.h"
#include "sub_404ed0.h"
#include "sub_406451.h"
#include "sub_4061df.h"

BOOL sub_406014_PluginInit(
)
{
    sub_404ed0_LogFormat(LOG_TAG(sub_406014_PluginInit), "Plugin System Version 0x%08X", 0x01000912);
    if (global_dd_408620_settings->db_0a8c_pluginDir.value[0] != '\0')
    {
        strcpy(global_db_402958_pluginDir, global_dd_408620_settings->db_0a8c_pluginDir.value);
    }
    sub_404ed0_LogFormat(LOG_TAG(sub_406014_PluginInit), "Plugin Directory is \"%s\"", global_db_402958_pluginDir);

    char fileName[0x104];
    WIN32_FIND_DATAA findFileData;

    wsprintfA(fileName, "%s\\*.dll", global_db_402958_pluginDir);

    // 0x3e8 Ϊ 1000
    global_dd_4085f0 = 0x3e8;
    InitializeCriticalSection(&global_dd_4085f8_criticalSection);

    assert(sizeof(findFileData) == 0x140);
    HANDLE handle = FindFirstFileA(fileName, &findFileData);
    if (handle == INVALID_HANDLE_VALUE)
    {
        sub_404ed0_LogFormat(LOG_TAG(sub_406014_PluginInit), "Error Searching Plugins");
        return FALSE;
    }
    else
    {
        do
        {
            sub_406451_LoadPlugin(findFileData.cFileName);
        } while (FindNextFileA(handle, &findFileData));

        FindClose(handle);

        sub_4061df_PluginListRun(RUN_PLUGIN_REASON_INIT);

        sub_404ed0_LogFormat(LOG_TAG(sub_406014_PluginInit), "Total %d Plugins Loaded", global_dd_408618_activePluginCount);

        return TRUE;
    }
}
