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
#include "functions/sub_406bab.h"
#include "functions/sub_406a68.h"


int sub_406ac0_FindArgumentNameIndex(const char* buffer)
{
    if (buffer == 0) {
        return -1;
    }

    if (buffer[0] != '-')
    {
        return -1;
    }

    int ebp_index = 0;
    if (global_dd_402ea8_CommandLineArgumentTable[0].category == (void*)ebp_index)
    {
        return -1;
    }

    // 指向去除“-”的参数名
    const char* ebx_argName = buffer + 1;

    struct string_index_item* esi_stringTableEntry = &global_dd_402ea8_CommandLineArgumentTable[0];
    do
    {
        const char* shortName = esi_stringTableEntry[0].shortName;
        if (lstrcmpiA(shortName, ebx_argName) == 0)
        {
            break;
        }
        const char* longName = esi_stringTableEntry[0].longName;
        if (lstrcmpiA(longName, ebx_argName) == 0)
        {
            break;
        }
        esi_stringTableEntry++;
        ebp_index++;
    } while (esi_stringTableEntry[0].category != NULL);

    // hoxily fix: 如果没有找到合适的参数名，则返回 -1。
    if (esi_stringTableEntry[0].category == NULL)
    {
        return -1;
    }
    else
    {
        return ebp_index;
    }
}

// 存储一条命令行参数
// @param i 命令行参数名在索引表的下标
// @param buffer 该命令行参数的值
BOOL sub_406b12_StoreCommandLineSetting(int i, char* buffer)
{
    assert(offsetof(struct string_index_item, type) == 0x10);
    DWORD type = global_dd_402ea8_CommandLineArgumentTable[i].type;
    DWORD offset = global_dd_402ea8_CommandLineArgumentTable[i].offset;
    if (type == ARG_TYPE_NO_PARAM)
    {
        BYTE toggle = global_dd_408620_settings->padding[offset];
        // 原代码是 xor byte ptr [eax], 1
        // 看起来会翻转原来已经设置好的配置项。
        // 预测：d2loader.exe -w -w  这样的启动参数，并不会窗口化运行，而是全屏运行。
        // 实测结果：确实是没有窗口化运行。
        global_dd_408620_settings->padding[offset] = !toggle;
    }
    else if (type == ARG_TYPE_UNSIGNED_SHORT_INT)
    {
        /*
        * 如果 base(_Radix) 为0，则使用 strSource 指向的字符串的初始字符来确定基。
        * 如果第一个字符为 0，且第二个字符不为“x”或“X”，则将该字符串视为八进制整数。
        * 如果第一个字符为“0”，且第二个字符为“x”或“X”，则将该字符串视为十六进制整数。
        * 如果第一个字符是“1”至“9”，则将该字符串视为十进制整数。
        */
        WORD ax = (WORD)strtoul(buffer, NULL, 0);
        WORD* ptr = (WORD*)(global_dd_408620_settings->padding + offset);
        *ptr = ax;
    }
    else if (type == ARG_TYPE_UNSIGNED_INT)
    {
        DWORD eax = strtoul(buffer, NULL, 0);
        DWORD* ptr = (DWORD*)(global_dd_408620_settings->padding + offset);
        *ptr = eax;
    }
    else
    {
        // 此时 type 表示字符串缓冲区的大小。
        int settingBufferSize = type;
        // 使用 (void) 丢弃 lstrcpynA 的返回结果。否则Visual Studio会有warning。
        (void)lstrcpynA(global_dd_408620_settings->padding + offset, buffer, settingBufferSize);
        // 强制设置缓冲区的最后一个字符为null结束符，以免溢出。
        global_dd_408620_settings->padding[offset + settingBufferSize - 1] = '\0';
    }

    return TRUE;
}

BOOL sub_406887_ProcessCommandLineArguments(const char* commandLine)
{
    char buffer[0x100];
    const char* edi_s = commandLine;
    do
    {
        edi_s = sub_406a68_CutFirstArgument(edi_s, buffer);
        int i = sub_406ac0_FindArgumentNameIndex(buffer);
        // test esi, esi
        // jl
        // 这样的组合表示检测esi是否为负数。
        if (i >= 0)
        {
            if (global_dd_402ea8_CommandLineArgumentTable[i].type != ARG_TYPE_NO_PARAM)
            {
                edi_s = sub_406a68_CutFirstArgument(edi_s, buffer);
            }

            sub_406b12_StoreCommandLineSetting(i, buffer);
        }
    } while (*edi_s != '\0');
    
    return TRUE;
}

BOOL sub_4068f2_LoadConfFile(const char* filename)
{
    // Buf 与 var_27FF 好像是同一个变量。var_27FF 等价于 Buf + 1
    char buffer[0x2800];

    // call __alloca_probe 是由于在栈上申请了大于 PAGE_SIZE 大小的空间，导致编译器插入了这段检查代码。
    // 用以确保程序运行到这里时，栈的空间足够。具体见 https://bbs.pediy.com/thread-145752.htm
    // 此处我们忽略它。
    
    if (filename == NULL)
    {
        return FALSE;
    }

    if (filename[0] == '\0')
    {
        return FALSE;
    }

    FILE* ebx_fp = fopen(filename, "r");
    if (ebx_fp == NULL)
    {
        return FALSE;
    }

    const size_t edi_bufferSize = sizeof(buffer);

    // fgets(buffer, edi_bufferSize, ebx_fp) 调用在汇编里被拆分成了两处。
    while (fgets(buffer, edi_bufferSize, ebx_fp))
    {
        // fgets 会将 \n 字符也读入 buffer 中。
        int eax_i = strlen(buffer);
        // dec eax 指令 不影响 CF 标志位。其余 OF, SF, ZF, AF, PF 标位都会根据计算结果相应设置。
        // js 指令表示jump short if sign (SF=1)。
        while (--eax_i >= 0)
        {
            if (buffer[eax_i] == '\r' || buffer[eax_i] == '\n')
            {
                continue;
            }
            else
            {
                break;
            }
        }

        buffer[eax_i + 1] = '\0';

        sub_406887_ProcessCommandLineArguments(buffer);
    }

    fclose(ebx_fp);

    if (global_dd_408620_settings->db_07ae_removeConfFile.value)
    {
        _unlink(filename);
    }

    return TRUE;
}

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

    // Render 是一个REG_DWORD类型的数据。可以在成功执行 D2VidTst.exe 之后，使用windows的注册表编辑器看到。
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
    // 汇编里出现 esp + 8 + hProcess, esp + 12 + hProcess, esp + 8 + priority，
    // 是因为这个函数不是bp-based栈帧，每次push一个东西，就会导致索引变化，相比bp-based要复杂一些。
    // IDA竟然分析出来了！
    
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
    // GetCurrentProcess 返回的是一个伪Handle，总是 (HANDLE)-1 即 0xffffffff。
    // 这并非出错了，而是故意这样设计的。
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
    // GetFileVersionInfoSizeA 总是会把 unusedHandle 设为 0
    DWORD unusedHandle;
    UINT len;
    VS_FIXEDFILEINFO* fileInfo;

    DWORD edi_size = GetFileVersionInfoSizeA(CSTR_GAME_DOT_EXE, &unusedHandle);
    void* ebx_ptr = VirtualAlloc(NULL, edi_size, MEM_COMMIT, PAGE_READWRITE);
    UINT esi = 0;
    if (ebx_ptr) {
        // esi 已经在上面被 xor esi, esi 置为0；
        if (GetFileVersionInfoA(CSTR_GAME_DOT_EXE, 0, edi_size, ebx_ptr))
        {
            if (VerQueryValueA(ebx_ptr, "\\", &fileInfo, &len))
            {
                if (fileInfo)
                {
                    assert(offsetof(VS_FIXEDFILEINFO, dwFileVersionLS) == 0x0c);
                    // 检查 game.exe 的FileVersion是否为 1.0.13.60，
                    // 由于 dwFileVersionMS 必定为 0x00010000，所以只检查了 dwFileVersionLS
                    if (fileInfo->dwFileVersionLS != 0x000d003c)
                    {
                        MessageBoxA(NULL, CSTR_D2_LOADER_DOES_NOT_RECOGNIZE_GAME_VERSION, CSTR_UNKNOW_VERSION, MB_OK | MB_ICONERROR);
                        ExitProcess(1);
                    }

                    // VS_FIXEDFILEINFO 结构的大小是 0x34
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

    // 在清理之前还有两段汇编，但是只有出口通向清理，却没有入口。略过。

    // VirtualFree 要求第一个参数不为 NULL，所以用if包裹一下。
    if (ebx_ptr)
    {
        VirtualFree(ebx_ptr, 0, MEM_RELEASE);
    }

    if (sub_40735e_CheckExpansion())
    {
        // 最高比特位 置1
        esi |= 0x80000000;
    }
    else
    {
        // 最高比特位 置0
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
            //esi 指向 entry->longName
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

BOOL sub_406451_LoadPlugin(const char* dllFileName)
{
    char libFileName[0x104];
    char buffer[0x104];
    
    if (dllFileName == NULL)
    {
        return FALSE;
    }

    sub_404ed0_LogFormat(LOG_TAG(sub_406451_LoadPlugin), "Loading Plugin %s", dllFileName);

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
