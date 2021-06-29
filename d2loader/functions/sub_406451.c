#include "sub_406451.h"
#include "../global-variables.h"
#include "sub_404ed0.h"
#include "sub_4065bd.h"

BOOL sub_406451_LoadPlugin(
    const char* dllFileName
)
{
    char libFileName[0x104];
    char buffer[0x104];

    if (dllFileName == NULL)
    {
        return FALSE;
    }

    sub_404ed0_LogFormat(LOG_TAG, "Loading Plugin %s", dllFileName);

    // \ 开头的路径表示当前驱动器的根目录
    // 第2个字符为冒号则表示指定了一个类似 C:\abc 这样的带驱动器的完整路径
    if (global_db_402958_pluginDir[0] == '\\' ||
        global_db_402958_pluginDir[1] == ':')
    {
        wsprintfA(libFileName, "%s\\%s", global_db_402958_pluginDir, dllFileName);
    }
    else
    {
        // 否则当作相对于当前目录处理
        GetCurrentDirectoryA(sizeof(buffer), buffer);
        wsprintfA(libFileName, "%s\\%s\\%s", buffer, global_db_402958_pluginDir, dllFileName);
    }

    HMODULE edi_dll = LoadLibraryA(libFileName);
    if (edi_dll == NULL)
    {
        DWORD errorCode = GetLastError();
        sub_404ed0_LogFormat(
            LOG_TAG,
            "errorCode: %u",
            errorCode);
        sub_404ed0_LogFormat(
            LOG_TAG,
            "Error Loading %s (%s)",
            dllFileName,
            libFileName);
        return FALSE;
    }

    fn_QueryInterface qi = (fn_QueryInterface)GetProcAddress(edi_dll, "QueryInterface");
    if (qi == NULL)
    {
        sub_404ed0_LogFormat(
            LOG_TAG,
            "Error QueryInterface on %s",
            dllFileName);
        FreeLibrary(edi_dll);
        return FALSE;
    }

    struct query_interface_result* ret = qi();
    if (ret == NULL || ret->magic != 0x44320000)
    {
        sub_404ed0_LogFormat(
            LOG_TAG,
            "Bad Plugin Interface for %s",
            dllFileName);
        FreeLibrary(edi_dll);
        return FALSE;
    }

    // cmp version, 10000000h
    // jb short loc_newFormatPlugin
    if (ret->version >= 0x10000000)
    {
        struct query_interface_result_old* retOld = (struct query_interface_result_old*)ret;
        sub_404ed0_LogFormat(
            LOG_TAG,
            "Old Format Plugin %s: \"%s\"",
            dllFileName,
            retOld->pluginName);
        DWORD gameProductVersionFlag = global_dd_408620_settings->dd_07b4_gameProductVersionFlag.value;
        retOld->init(gameProductVersionFlag);
        return TRUE;
    }
    else
    {
        const int targetVersion = 0x01000912;
        if (ret->version != targetVersion)
        {
            sub_404ed0_LogFormat(
                LOG_TAG,
                "Plugin %s Version Mismatch %d/%d",
                dllFileName,
                ret->version,
                targetVersion);

            FreeLibrary(edi_dll);
            return FALSE;
        }
        else
        {
            return sub_4065bd_AddPlugin(edi_dll, dllFileName, ret);
        }
    }
}
