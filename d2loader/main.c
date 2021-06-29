#include <Windows.h>
#include <assert.h>
#include <stdio.h>
#include "logger.h"
#include "constant-strings.h"
#include "constants.h"
#include "data-types.h"
#include "global-variables.h"
#include "functions/sub_404ed0.h"
#include "functions/sub_40735e.h"
#include "functions/sub_406887.h"



BOOL sub_406bb9_LoadVideoConfigFromRegistry()
{
    DWORD data;
    HKEY phkResult;
    DWORD cbData;
    DWORD type;

    LSTATUS ret = RegOpenKeyExA(
        HKEY_CURRENT_USER,
        "Software\\Blizzard Entertainment\\Diablo II\\VideoConfig",
        0,
        KEY_READ,
        &phkResult);
    
    if (ret != ERROR_SUCCESS)
    {
        return FALSE;
    }

    // Render ��һ��REG_DWORD���͵����ݡ������ڳɹ�ִ�� D2VidTst.exe ֮��ʹ��windows��ע���༭��������
    cbData = sizeof(data);
    ret = RegQueryValueExA(
        phkResult,
        "Render",
        NULL,
        &type,
        (LPBYTE)&data,
        &cbData);
    if (ret != ERROR_SUCCESS)
    {
        RegCloseKey(phkResult);
        return FALSE;
    }
    else
    {
        if (data == 1)
        {
            global_dd_408620_settings->padding[0x0008] = TRUE;
        }
        else if (data == 2)
        {
            global_dd_408620_settings->padding[0x0006] = TRUE;
        }
        else if (data == 3)
        {
            global_dd_408620_settings->padding[0x0005] = TRUE;
        }
        else if (data == 4)
        {
            global_dd_408620_settings->padding[0x0004] = TRUE;
        }
    }
    RegCloseKey(phkResult);
    return TRUE;
}

BOOL sub_407bb9_InitializeProcessPriority(HANDLE hProcess, const char* priority)
{
    // �������� esp + 8 + hProcess, esp + 12 + hProcess, esp + 8 + priority��
    // ����Ϊ�����������bp-basedջ֡��ÿ��pushһ���������ͻᵼ�������仯�����bp-basedҪ����һЩ��
    // IDA��Ȼ���������ˣ�
    
    if (!priority)
    {
        return FALSE;
    }

    if (!hProcess)
    {
        return FALSE;
    }

    DWORD priorityClass;
    if (_stricmp(priority, CSTR_PROCESS_PRIORITY_IDLE) == 0)
    {
        priorityClass = IDLE_PRIORITY_CLASS;
    }
    else if (_stricmp(priority, CSTR_PROCESS_PRIORITY_NORMAL) == 0)
    {
        priorityClass = NORMAL_PRIORITY_CLASS;
    }
    else if (_stricmp(priority, CSTR_PROCESS_PRIORITY_BELOW_NORMAL) == 0)
    {
        priorityClass = BELOW_NORMAL_PRIORITY_CLASS;
    }
    else if (_stricmp(priority, CSTR_PROCESS_PRIORITY_ABOVE_NORMAL) == 0)
    {
        priorityClass = ABOVE_NORMAL_PRIORITY_CLASS;
    }
    else if (_stricmp(priority, CSTR_PROCESS_PRIORITY_HIGH) == 0)
    {
        priorityClass = HIGH_PRIORITY_CLASS;
    }
    else if (_stricmp(priority, CSTR_PROCESS_PRIORITY_REALTIME) == 0)
    {
        priorityClass = REALTIME_PRIORITY_CLASS;
    }
    else
    {
        return FALSE;
    }
    
    return SetPriorityClass(hProcess, priorityClass);
}

#include "functions/sub_4069d8.h"
#include "functions/sub_4068f2.h"

BOOL sub_406803_InitializeSettings()
{
    assert(sizeof(union program_setting_store) == 0xc94);
    global_dd_408620_settings = malloc(sizeof(union program_setting_store));
    if (!global_dd_408620_settings)
    {
        return FALSE;
    }
    sub_4069d8_InitializeDefaultSettings();
    char* commandLine = GetCommandLineA();
    if (!sub_406887_ProcessCommandLineArguments(commandLine))
    {
        return FALSE;
    }
    sub_4068f2_LoadConfFile(global_dd_408620_settings->db_0884_confFile.value);

    char* dstStr = global_dd_408620_settings->db_0004_str.value;
    if (dstStr[0] == 0 &&
        dstStr[1] == 0 &&
        dstStr[2] == 0 &&
        dstStr[3] == 0 &&
        dstStr[4] == 0)
    {
        sub_406bb9_LoadVideoConfigFromRegistry();
    }

    char* priority = global_dd_408620_settings->db_07bc_processPriority.value;
    // GetCurrentProcess ���ص���һ��αHandle������ (HANDLE)-1 �� 0xffffffff��
    // �Ⲣ�ǳ����ˣ����ǹ���������Ƶġ�
    HANDLE hProcess = GetCurrentProcess();
    sub_407bb9_InitializeProcessPriority(hProcess, priority);

    return TRUE;
}

void sub_404eb1_SetLogFile(FILE* fp)
{
    global_dd_40858c_logFile = fp;
}

void sub_404ec5_EnableLog(DWORD logType)
{
    global_dd_408590_logFlag |= logType;
}

void sub_406c59_CheckGameExeVersion()
{
    // GetFileVersionInfoSizeA ���ǻ�� unusedHandle ��Ϊ 0
    DWORD unusedHandle;
    UINT len;
    VS_FIXEDFILEINFO* fileInfo;

    DWORD edi_size = GetFileVersionInfoSizeA(CSTR_GAME_DOT_EXE, &unusedHandle);
    void* ebx_ptr = VirtualAlloc(NULL, edi_size, MEM_COMMIT, PAGE_READWRITE);
    UINT esi = 0;
    if (ebx_ptr) {
        // esi �Ѿ������汻 xor esi, esi ��Ϊ0��
        if (GetFileVersionInfoA(CSTR_GAME_DOT_EXE, 0, edi_size, ebx_ptr))
        {
            if (VerQueryValueA(ebx_ptr, "\\", &fileInfo, &len))
            {
                if (fileInfo)
                {
                    assert(offsetof(VS_FIXEDFILEINFO, dwFileVersionLS) == 0x0c);
                    // ��� game.exe ��FileVersion�Ƿ�Ϊ 1.0.13.60��
                    // ���� dwFileVersionMS �ض�Ϊ 0x00010000������ֻ����� dwFileVersionLS
                    if (fileInfo->dwFileVersionLS != 0x000d003c)
                    {
                        MessageBoxA(NULL, CSTR_D2_LOADER_DOES_NOT_RECOGNIZE_GAME_VERSION, CSTR_UNKNOW_VERSION, MB_OK | MB_ICONERROR);
                        ExitProcess(1);
                    }

                    // VS_FIXEDFILEINFO �ṹ�Ĵ�С�� 0x34
                    assert(sizeof(VS_FIXEDFILEINFO) == 0x34);
                    if (len == sizeof(VS_FIXEDFILEINFO))
                    {
                        assert(offsetof(VS_FIXEDFILEINFO, dwProductVersionMS) == 0x10);
                        UINT version = fileInfo->dwProductVersionMS;
                        version >>= 16;
                        esi = version & 0xff;
                        version = fileInfo->dwProductVersionMS & 0xff;
                        esi <<= 8;
                        esi |= version;

                        assert(offsetof(VS_FIXEDFILEINFO, dwProductVersionLS) == 0x14);
                        version = fileInfo->dwProductVersionLS;
                        UINT eax = fileInfo->dwProductVersionLS & 0xff;
                        version >>= 16;
                        version = version & 0xff;
                        esi <<= 8;
                        esi |= version;
                        esi <<= 8;
                        esi |= eax;
                    }
                }
            }
        }
    }

    // ������֮ǰ�������λ�࣬����ֻ�г���ͨ������ȴû����ڡ��Թ���

    // VirtualFree Ҫ���һ��������Ϊ NULL��������if����һ�¡�
    if (ebx_ptr)
    {
        VirtualFree(ebx_ptr, 0, MEM_RELEASE);
    }

    if (sub_40735e_CheckExpansion())
    {
        // ��߱���λ ��1
        esi |= 0x80000000;
    }
    else
    {
        // ��߱���λ ��0
        esi &= 0x7fffffff;
    }
    global_dd_408620_settings->dd_07b4_gameProductVersionFlag.value = esi;
}

BOOL sub_4066dc_PrintParametersTable(FILE* fp)
{
    if (fp == NULL)
    {
        return FALSE;
    }

    struct string_index_item* entry = &global_dd_402ea8_CommandLineArgumentTable[0];
    if (entry->category != NULL)
    {
        do
        {
            //esi ָ�� entry->longName
            fprintf(fp, "%-12s %-12s 0x%08X\n", entry->category, entry->longName, entry->offset);
            entry++;
        } while (entry->category != NULL);
    }

    fflush(fp);
    return TRUE;
}

BOOL sub_4065bd_AddPlugin(
    HMODULE dllModule,
    const char* dllFileName,
    struct query_interface_result* result)
{
    CRITICAL_SECTION* edi_criticalSection = &global_dd_4085f8_criticalSection;
    EnterCriticalSection(edi_criticalSection);
    {
        if (global_dd_408614_count >= global_dd_40861c_capacity)
        {
            global_dd_40861c_capacity += 0xa;
            assert(sizeof(struct loaded_plugin_item) == 20);
            size_t newSize = global_dd_40861c_capacity * sizeof(struct loaded_plugin_item);
            // realloc ���ܻ᷵��NULL��Visual StudioҪ���鷵��ֵ��
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

BOOL sub_406451_LoadPlugin(const char* dllFileName)
{
    char libFileName[0x104];
    char buffer[0x104];
    
    if (dllFileName == NULL)
    {
        return FALSE;
    }

    sub_404ed0_LogFormat(LOG_TAG(sub_406451_LoadPlugin), "Loading Plugin %s", dllFileName);

    // \ ��ͷ��·����ʾ��ǰ�������ĸ�Ŀ¼
    // ��2���ַ�Ϊð�����ʾָ����һ������ C:\abc �����Ĵ�������������·��
    if (global_db_402958_pluginDir[0] == '\\' ||
        global_db_402958_pluginDir[1] == ':')
    {
        wsprintfA(libFileName, "%s\\%s", global_db_402958_pluginDir, dllFileName);
    }
    else
    {
        // ����������ڵ�ǰĿ¼����
        GetCurrentDirectoryA(sizeof(buffer), buffer);
        wsprintfA(libFileName, "%s\\%s\\%s", buffer, global_db_402958_pluginDir, dllFileName);
    }

    HMODULE edi_dll = LoadLibraryA(libFileName);
    if (edi_dll == NULL)
    {
        DWORD errorCode = GetLastError();
        LogFormat("errorCode: %u", errorCode);
        sub_404ed0_LogFormat(
            LOG_TAG(sub_406451_LoadPlugin),
            "Error Loading %s (%s)",
            dllFileName,
            libFileName);
        return FALSE;
    }

    fn_QueryInterface qi = (fn_QueryInterface)GetProcAddress(edi_dll, "QueryInterface");
    if (qi == NULL)
    {
        sub_404ed0_LogFormat(
            LOG_TAG(sub_406451_LoadPlugin),
            "Error QueryInterface on %s",
            dllFileName);
        FreeLibrary(edi_dll);
        return FALSE;
    }

    struct query_interface_result* ret = qi();
    if (ret == NULL || ret->magic != 0x44320000)
    {
        sub_404ed0_LogFormat(
            LOG_TAG(sub_406451_LoadPlugin),
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
            LOG_TAG(sub_406451_LoadPlugin),
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
                LOG_TAG(sub_406451_LoadPlugin),
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

void sub_40a4e5()
{
    //TODO
}

BOOL sub_406373_RunPlugin(struct loaded_plugin_item* plugin, DWORD reasonFlag);

BOOL sub_4063f9_UnloadPlugin(struct loaded_plugin_item* plugin)
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
        LOG_TAG(sub_4063f9_UnloadPlugin),
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

BOOL sub_406373_RunPlugin(struct loaded_plugin_item* plugin, DWORD reasonFlag)
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

int sub_4061df_PluginListRun(DWORD reasonFlag)
{
    if (global_dd_408610_plugins == NULL)
    {
        if (reasonFlag & RUN_PLUGIN_REASON_ENTER_MAIN_MENU)
        {
            sub_40a4e5();
        }

        return 0;
    }

    char reasonString[0x100];
    reasonString[0] = '\0';
    if (reasonFlag & RUN_PLUGIN_REASON_INIT)
    {
        strcat(reasonString, "REASON_INIT |");
    }
    if (reasonFlag & RUN_PLUGIN_REASON_LEAVE_GAME)
    {
        strcat(reasonString, "REASON_LEAVE_GAME |");
    }
    if (reasonFlag & RUN_PLUGIN_REASON_LEAVE_CHANNEL)
    {
        strcat(reasonString, "REASON_LEAVE_CHANNEL |");
    }
    if (reasonFlag & RUN_PLUGIN_REASON_LEAVE_MAIN_MENU)
    {
        strcat(reasonString, "REASON_LEAVE_MAIN_MENU |");
    }
    if (reasonFlag & RUN_PLUGIN_REASON_ENTER_MAIN_MENU)
    {
        sub_40a4e5();
        strcat(reasonString, "REASON_ENTER_MAIN_MENU |");
    }
    if (reasonFlag & RUN_PLUGIN_REASON_ENTER_CHANNEL)
    {
        strcat(reasonString, "REASON_ENTER_CHANNEL |");
    }
    if (reasonFlag & RUN_PLUGIN_REASON_CLEANUP)
    {
        strcat(reasonString, "REASON_CLEANUP |");
    }
    if (reasonFlag & RUN_PLUGIN_REASON_ENTER_GAME)
    {
        strcat(reasonString, "REASON_ENTER_GAME |");
    }
    // ���涼�� test bl, ������
    // �������� test bh, 1
    // ������������ĵ�8λΪ��
    if (reasonFlag & RUN_PLUGIN_REASON_ERROR_CLEANUP)
    {
        strcat(reasonString, "REASON_ERROR_CLEANUP |");
    }

    if (reasonString[0] == '\0')
    {
        // edi ��ĿǰΪֹ���� 0�����strcpy����û�����塣
        // ��archlinux���������Ĳ����ᵼ�¶δ��󡣿�������δ�������ǿ�ƽ������̣�
        //strcpy(reasonString, NULL);

        // fix by hoxily@qq.com
        strcpy(reasonString, "");
    }
    else
    {
        size_t len = strlen(reasonString);
        // ����о�Ӧ��Ҫ len - 2 ���ܰ�ĩβ�Ŀո�����߳�ȥ��
        // fix by hoxily@qq.com
        reasonString[len - 2] = '\0';
    }

    sub_404ed0_LogFormat(
        LOG_TAG(sub_4061df_PluginListRun),
        "Call All Plugins With %s",
        reasonString);

    int runCount = 0;
    EnterCriticalSection(&global_dd_4085f8_criticalSection);
    {
        // fix by hoxily@qq.com: ����Ӧ���� global_dd_408614_count ��
        // ��Ϊ�� RunPlugin �����ڲ����п��ܶ�plugin��ж�ز���������activePluginCountֵ��
        for (DWORD ebx_i = 0; ebx_i < global_dd_408614_count; ebx_i++)
        {
            BOOL ret = sub_406373_RunPlugin(&global_dd_408610_plugins[ebx_i], reasonFlag);
            if (ret)
            {
                runCount++;
            }
        }
    }
    LeaveCriticalSection(&global_dd_4085f8_criticalSection);
    
    return runCount;
}

BOOL sub_406014_PluginInit()
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


BOOL sub_4053b3_IsWin32NtPlatform()
{
    OSVERSIONINFOA version;
    memset(&version, 0, sizeof(version));
    assert(offsetof(OSVERSIONINFOA, dwOSVersionInfoSize) == 0);
    version.dwOSVersionInfoSize = sizeof(version);

    // ��ʱ���� obsolete������visual studio����ʧ�ܡ�
#pragma warning(disable: 4996)
    BOOL ret = GetVersionExA(&version);
#pragma warning(default: 4996)

    if (ret)
    {
        assert(offsetof(OSVERSIONINFOA, dwPlatformId) == 0x10);
        if (version.dwPlatformId == VER_PLATFORM_WIN32_NT)
        {
            return TRUE;
        }
    }
    return FALSE;
}

IMAGE_IMPORT_DESCRIPTOR* sub_40532e(HMODULE hModule, const char* hookDll)
{
    if (hookDll == NULL || hModule == NULL)
    {
        SetLastErrorEx(ERROR_INVALID_PARAMETER, 1);
        return NULL;
    }
    
    assert(sizeof(IMAGE_DOS_HEADER) == 0x40);
    if (IsBadReadPtr(hModule, sizeof(IMAGE_DOS_HEADER)))
    {
        SetLastErrorEx(ERROR_INVALID_PARAMETER, 1);
        return NULL;
    }

    WORD magicWord = *((WORD*)hModule);
    // DOS signature (MZ)
    if (magicWord != IMAGE_DOS_SIGNATURE)
    {
        SetLastErrorEx(ERROR_INVALID_PARAMETER, 1);
        return NULL;
    }

    IMAGE_DOS_HEADER* header = (IMAGE_DOS_HEADER*)hModule;
    assert(offsetof(IMAGE_DOS_HEADER, e_lfanew) == 0x3c);
    LONG aNew = header->e_lfanew;
    void* newPtr = (char*)hModule + aNew;

    assert(sizeof(IMAGE_NT_HEADERS32) == 0xf8);
    if (IsBadReadPtr(newPtr, sizeof(IMAGE_NT_HEADERS32)))
    {
        SetLastErrorEx(ERROR_INVALID_PARAMETER, 1);
        return NULL;
    }
    IMAGE_NT_HEADERS32* ntHeader = (IMAGE_NT_HEADERS32*)newPtr;
    if (ntHeader->Signature != IMAGE_NT_SIGNATURE)
    {
        SetLastErrorEx(ERROR_INVALID_PARAMETER, 1);
        return NULL;
    }
    char* ptr_0x80 = (char*)&ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
    char* ptr_0 = (char*)ntHeader;
    assert(ptr_0x80 - ptr_0 == 0x80);

    IMAGE_DATA_DIRECTORY importDataDirectory = ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
    if (importDataDirectory.VirtualAddress == 0)
    {
        return NULL;
    }

    IMAGE_IMPORT_DESCRIPTOR* importDescriptor = (IMAGE_IMPORT_DESCRIPTOR*)((char*)hModule + importDataDirectory.VirtualAddress);
    while (importDescriptor->Name)
    {
        const char* functionName = (const char*)hModule + importDescriptor->Name;
        if (lstrcmpiA(functionName, hookDll) == 0)
        {
            break;
        }

        importDescriptor++;
    }
    /*
    * �����е������ת����Ԫ�����ʵ�֡�
    * 
    * esi ��ǰΪ importDescriptor ָ��
    * mov eax, [esi + 0ch]
    * neg eax
    * sbb eax, eax
    * and eax, esi
    * ...
    * retn
    * 
    * �����neg��sbb������ϴ����������һ�������ǽ�ĳ��������Ϊ0����0xffffffff��
    * ��� and ���㣬���൱�ڸ��� importDescriptor->Name �Ƿ�Ϊ 0��������Ҫ���� importDescriptor ���� NULL��
    */

    return importDescriptor->Name ? importDescriptor : NULL;
}

BOOL sub_40513a(
    HMODULE hModule,
    const char* hookDll,
    struct hook_search_item* functionNameList,
    void* null1,
    void* null2)
{
    if (hookDll == NULL)
    {
        SetLastErrorEx(ERROR_INVALID_PARAMETER, 1);
        return FALSE;
    }

    if (functionNameList == NULL)
    {
        SetLastErrorEx(ERROR_INVALID_PARAMETER, 1);
        return FALSE;
    }

    DWORD functionNameListCount = 0;
    for (struct hook_search_item* eax_item = functionNameList; eax_item->functionName != (const char*)-1; eax_item++)
    {
        functionNameListCount++;
    }

    if (functionNameListCount == 0)
    {
        SetLastErrorEx(ERROR_INVALID_PARAMETER, 1);
        return FALSE;
    }

    if (null1 != NULL)
    {
        if (IsBadWritePtr(null1, functionNameListCount * 4))
        {
            SetLastErrorEx(ERROR_INVALID_PARAMETER, 1);
            return FALSE;
        }
    }

    if (null2 != NULL)
    {
        if (IsBadWritePtr(null2, 4))
        {
            SetLastErrorEx(ERROR_INVALID_PARAMETER, 1);
            return FALSE;
        }
    }

    if (functionNameListCount != 0)
    {
        DWORD ebx_count = 0;
        struct hook_search_item* esi_item = functionNameList;
        do
        {
            if (esi_item->functionName == NULL)
            {
                return FALSE;
            }
            void* ptr = esi_item->functionProcAddress;
            if (ptr != NULL)
            {
                if (IsBadCodePtr(ptr))
                {
                    return FALSE;
                }
            }

            ebx_count++;
            esi_item++;
        } while (ebx_count < functionNameListCount);
    }

    BOOL isWin32NtPlatform = sub_4053b3_IsWin32NtPlatform();
    if (!isWin32NtPlatform)
    {
        if ((void*)hModule >= (void*)0x80000000)
        {
            SetLastErrorEx(ERROR_INVALID_HANDLE, 1);
            return FALSE;
        }
    }

    if (null1 != NULL)
    {
        memset(null1, 0, functionNameListCount * 4);
    }

    if (null2 != NULL)
    {
        memset(null2, 0, 4);
    }

    IMAGE_IMPORT_DESCRIPTOR* importDescriptor = sub_40532e(hModule, hookDll);
    if (importDescriptor == NULL)
    {
        return FALSE;
    }
    
    assert(offsetof(IMAGE_IMPORT_DESCRIPTOR, OriginalFirstThunk) == 0);
    assert(offsetof(IMAGE_IMPORT_DESCRIPTOR, FirstThunk) == 0x10);
    IMAGE_THUNK_DATA32* edi_originalThunkData = (IMAGE_THUNK_DATA32*)((char*)hModule + importDescriptor->OriginalFirstThunk);
    IMAGE_THUNK_DATA32* esi_thunkData = (IMAGE_THUNK_DATA32*)((char*)hModule + importDescriptor->FirstThunk);
    
    assert(sizeof(IMAGE_THUNK_DATA32) == sizeof(DWORD));
    // fix by hoxily@qq.com, �������ѭ�����Ѿ�����ʹ�� hookDll ������ֵ����������һ��DWORD��С�ı������á�
    // Ϊ�˷��㣬�������Ƕ��ⶨ��һ�� isByOrdinal ������
    BOOL isByOrdinal;
    IMAGE_IMPORT_BY_NAME* var_c = NULL;
    while (*(DWORD*)edi_originalThunkData != 0)
    {
        if (IMAGE_SNAP_BY_ORDINAL32(edi_originalThunkData->u1.Ordinal))
        {
            isByOrdinal = TRUE;
        }
        else
        {
            isByOrdinal = FALSE;
            var_c = (IMAGE_IMPORT_BY_NAME*)((char*)hModule + edi_originalThunkData->u1.AddressOfData);
            if (var_c->Name[0] == '\0')
            {
                // fix by hoxily@qq.com, ����û�ж� edi ������������ȥѭ����ڡ�
                // һ������һ����Ordinal������Name�������ǿյ���ʱ�����ᵼ����ѭ����
                edi_originalThunkData++;
                // esi Ҳ��Ҫ������
                esi_thunkData++;
                continue;
            }
        }

        if (functionNameListCount <= 0)
        {
            edi_originalThunkData++;
            esi_thunkData++;
            continue;
        }

        struct hook_search_item* eax_item = functionNameList;
        DWORD ebx_i = 0;
        BOOL hookFlag = FALSE;
        for (; ebx_i < functionNameListCount; ebx_i++)
        {
            if (!isByOrdinal)
            {
                if (((DWORD)eax_item->functionName & 0x80000000) == 0)
                {
                    if (lstrcmpiA(eax_item->functionName, var_c->Name) == 0)
                    {
                        if (functionNameList[ebx_i].functionProcAddress == NULL)
                        {
                            break;
                        }
                        else
                        {
                            hookFlag = TRUE;
                            break;
                        }
                    }
                }
            }
            else
            {
                if (edi_originalThunkData->u1.Ordinal == (DWORD)eax_item->functionName)
                {
                    hookFlag = TRUE;
                    break;
                }
            }
            eax_item++;
        }

        if (hookFlag)
        {
            MEMORY_BASIC_INFORMATION memoryBasicInfo;
            DWORD oldProtect;
            assert(sizeof(memoryBasicInfo) == 0x1c);
            VirtualQuery(
                esi_thunkData,
                &memoryBasicInfo,
                sizeof(memoryBasicInfo)
            );
            VirtualProtect(
                memoryBasicInfo.BaseAddress,
                memoryBasicInfo.RegionSize,
                PAGE_EXECUTE_READWRITE,
                &memoryBasicInfo.Protect
            );
            if (null1 != NULL)
            {
                DWORD* ptr = (DWORD*)null1;
                ptr[ebx_i] = esi_thunkData->u1.Function;
            }

            esi_thunkData->u1.Function = (DWORD)functionNameList[ebx_i].functionProcAddress;
            VirtualProtect(
                memoryBasicInfo.BaseAddress,
                memoryBasicInfo.RegionSize,
                memoryBasicInfo.Protect,
                &oldProtect
            );
            if (null2 != NULL)
            {
                DWORD* ptr = (DWORD*)null2;
                (*ptr)++;
            }
        }

        edi_originalThunkData++;
        esi_thunkData++;
    }

    SetLastError(ERROR_SUCCESS);
    return TRUE;
}

BOOL sub_4054fd_HookDll()
{
    HMODULE hModuleKernel32 = GetModuleHandleA("Kernel32.dll");
    assert(hModuleKernel32 != NULL);
    global_dd_4085a0_LoadLibraryA = (fn_LoadLibraryA)GetProcAddress(hModuleKernel32, "LoadLibraryA");
    global_dd_4085a4_GetModuleFileNameA = (fn_GetModuleFileNameA)GetProcAddress(hModuleKernel32, "GetModuleFileNameA");
    
    HMODULE hModuleUser32 = GetModuleHandleA("User32.dll");
    assert(hModuleUser32 != NULL);
    global_dd_4085a8_FindWindowA = (fn_FindWindowA)GetProcAddress(hModuleUser32, "FindWindowA");
    
    char filename[0x104];
    GetModuleFileNameA(NULL, filename, sizeof(filename));
    global_dd_408598_moduleD2LoaderFileName = _strdup(filename);

    char* filePart;
    GetFullPathNameA("game.exe", sizeof(filename), filename, &filePart);
    global_dd_40859c_gameDotExeFileName = _strdup(filename);

    /*
    * ����жδ���Ƚ�����⡣
    * mov [esp + 118h + lpLibFileName], offset LibFileName ; "bnclient.dll"
    * 
    * ���е� lpLibFileName ��ʵ���� -118h���൱�ڰѵ�ǰջ��Ԫ�ظ�������ַ��������׵�ַ��
    * 
    * �������������֮ǰ������ __cdecl �淶�� _strdup ��������û��ƽ��ջ��
    * Ҳ���൱�� pop ecx; mov ecx, offset LibFileName; push ecx;
    */
    global_dd_4085b0_moduleBnClient = LoadLibraryA("bnclient.dll");
    global_dd_4085ac_moduleD2Gfx = LoadLibraryA("d2gfx.dll");
    global_dd_4085b4_moduleD2Net = LoadLibraryA("d2net.dll");

    BOOL ret1 = sub_40513a(
        global_dd_4085b0_moduleBnClient,
        "Kernel32.dll",
        &global_dd_4023f0[0],
        NULL,
        NULL
    );
    BOOL ret2 = sub_40513a(
        global_dd_4085ac_moduleD2Gfx,
        "User32.dll",
        &global_dd_402408[0],
        NULL,
        NULL
    );
    
    return ret1 && ret2;
}

BOOL sub_40a480()
{
    //TODO
}

void sub_4057a8()
{
    //TODO
}

void sub_40a600_Cleanup()
{
    //TODO
}

void sub_4071a5()
{
    //TODO
}

void sub_405af1()
{
    //TODO
}

void sub_406175()
{
    //TODO
}

void sub_405602()
{
    //TODO
}

void sub_4069bc()
{
    //TODO
}

void sub_404d77_GameClientLoop()
{
    //TODO
}

#include "functions/sub_404b60.h"

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd)
{
    InitLogFile();

    return sub_404b60_WinMain(
        hInstance,
        hPrevInstance,
        lpCmdLine,
        nShowCmd
    );
}
