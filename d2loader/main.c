#include <Windows.h>
#include <assert.h>
#include <stdio.h>
#include "logger.h"
#include "constant-strings.h"
#include "constants.h"
#include "data-types.h"
#include "global-variables.h"
#include "functions/sub_404ed0.h"
#include "functions/sub_406451.h"
#include "functions/sub_40a4e5.h"


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
    // 上面都是 test bl, 立即数
    // 这里则是 test bh, 1
    // 因此这里的掩码的低8位为零
    if (reasonFlag & RUN_PLUGIN_REASON_ERROR_CLEANUP)
    {
        strcat(reasonString, "REASON_ERROR_CLEANUP |");
    }

    if (reasonString[0] == '\0')
    {
        // edi 到目前为止还是 0，这个strcpy好像没有意义。
        // 在archlinux下做这样的操作会导致段错误。看起来这段代码是想强制结束进程？
        //strcpy(reasonString, NULL);

        // fix by hoxily@qq.com
        strcpy(reasonString, "");
    }
    else
    {
        size_t len = strlen(reasonString);
        // 这里感觉应该要 len - 2 才能把末尾的空格和竖线除去。
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
        // fix by hoxily@qq.com: 这里应该是 global_dd_408614_count 。
        // 因为在 RunPlugin 函数内部，有可能对plugin做卸载操作，减少activePluginCount值。
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

    // 0x3e8 为 1000
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

    // 临时禁用 obsolete。否则visual studio编译失败。
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
    * 这里有点妙。无跳转的三元运算符实现。
    * 
    * esi 当前为 importDescriptor 指针
    * mov eax, [esi + 0ch]
    * neg eax
    * sbb eax, eax
    * and eax, esi
    * ...
    * retn
    * 
    * 这里的neg、sbb组合与上次遇到的情况一样。都是将某个数，变为0或者0xffffffff。
    * 最后 and 运算，就相当于根据 importDescriptor->Name 是否为 0，来决定要返回 importDescriptor 还是 NULL。
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
    // fix by hoxily@qq.com, 下面这个循环里已经不再使用 hookDll 参数的值，把它当做一个DWORD大小的变量来用。
    // 为了方便，这里我们额外定义一个 isByOrdinal 变量。
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
                // fix by hoxily@qq.com, 这里没有对 edi 做增长，就跳去循环入口。
                // 一旦遇到一个非Ordinal，但是Name数据又是空的项时，将会导致死循环。
                edi_originalThunkData++;
                // esi 也需要增长。
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
    * 这儿有段代码比较难理解。
    * mov [esp + 118h + lpLibFileName], offset LibFileName ; "bnclient.dll"
    * 
    * 其中的 lpLibFileName 其实就是 -118h，相当于把当前栈顶元素赋予这个字符串常量首地址。
    * 
    * 最神奇的是在这之前调用了 __cdecl 规范的 _strdup 函数，还没有平衡栈。
    * 也就相当于 pop ecx; mov ecx, offset LibFileName; push ecx;
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
