#include <Windows.h>
#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include "logger.h"
#include "constant-strings.h"

/*
 * 数据类型前缀：
 * db: 1 字节
 * dw: 2 字节
 * dd: 4 字节
 */

/* global variable */
union version_info* global_dd_408620_Dst;
FILE* global_dd_408588_logFile;
FILE* global_dd_40858c_logFile;

#define LOG_TYPE_FILE 0x20
#define LOG_TYPE_CONSOLE 0x10

DWORD global_dd_408590_logFlag;

// 该命令行参数无具体参数值
#define ARG_TYPE_NO_PARAM 1
#define ARG_TYPE_04 4

#pragma pack(1)
struct string_index_item
{
    // 参数的短名
    const char* shortName;
    // 参数分类
    const char* category;
    // 参数的长名
    const char* longName;
    // 参数的长名的长度
    DWORD length;
    // 参数的类型
    DWORD type;
};

struct string_index_item global_dd_402ea8_indexTable[93] = {
    { "exp", "MAIN", "expansion", 0x0, ARG_TYPE_NO_PARAM },
    { "w", "VIDEO", "window", 0x4, ARG_TYPE_NO_PARAM },
    { "glide", "VIDEO", "glide", 0x6, ARG_TYPE_NO_PARAM },
    { "opengl", "VIDEO", "opengl", 0x7, ARG_TYPE_NO_PARAM },
    { "d3d", "VIDEO", "d3d", 0x9, ARG_TYPE_NO_PARAM },
    { "rave", "VIDEO", "rave", 0x8, ARG_TYPE_NO_PARAM },
    { "per", "VIDEO", "perspective", 0x9, ARG_TYPE_NO_PARAM },
    { "lq", "VIDEO", "lowquality", 0xa, ARG_TYPE_NO_PARAM },
    { "gamma", "VIDEO", "gamma", 0xc, 0x4 },
    { "vsync", "VIDEO", "vsync", 0x10, ARG_TYPE_NO_PARAM },
    { "fr", "VIDEO", "framerate", 0x10, 0x4 },
    { "s", "NETWORK", "serverip", 0x33, 0x18 },
    { "gametype", "NETWORK", "gametype", 0x14, 0x4 },
    { "joinid", "NETWORK", "joinid", 0x18, 0x2 },
    { "gamename", "NETWORK", "gamename", 0x1a, 0x18 },
    { "bn", "NETWORK", "battlenetip", 0x4b, 0x18 },
    { "mcpip", "NETWORK", "mcpip", 0x63, 0x18 },
    { "nopk", "NETWORK", "nopk", 0x7e, ARG_TYPE_NO_PARAM },
    { "openc", "NETWORK", "openc", 0x7f, ARG_TYPE_NO_PARAM },
    { "arena", "GAME", "arena", 0x202, 0x2 },
    { "difficulty", "GAME", "difficulty", 0x20b, ARG_TYPE_NO_PARAM },
    { "txt", "GAME", "text", 0x211, ARG_TYPE_NO_PARAM },
    { "ama", "CHARACTER", "ama", 0x81, ARG_TYPE_NO_PARAM },
    { "pal", "CHARACTER", "pal", 0x81, ARG_TYPE_NO_PARAM },
    { "sor", "CHARACTER", "sor", 0x82, ARG_TYPE_NO_PARAM },
    { "nec", "CHARACTER", "nec", 0x83, ARG_TYPE_NO_PARAM },
    { "bar", "CHARACTER", "bar", 0x84, ARG_TYPE_NO_PARAM },
    { "dru", "CHARACTER", "dru", 0x85, ARG_TYPE_NO_PARAM },
    { "asn", "CHARACTER", "asn", 0x86, ARG_TYPE_NO_PARAM },
    { "i", "CHARACTER", "invincible", 0x87, ARG_TYPE_NO_PARAM },
    { "bnacct", "CHARACTER", "bnacct", 0x88, 0x18 },
    { "bnpass", "CHARACTER", "bnpass", 0xa0, 0x18 },
    { "name", "CHARACTER", "name", 0xb9, 0x18 },
    { "realm", "CHARACTER", "realm", 0xd1, 0x18 },
    { "ctemp", "CHARACTER", "ctemp", 0x1e9, ARG_TYPE_NO_PARAM },
    { "charclass", "CHARACTER", "charclass", 0x1e9, ARG_TYPE_NO_PARAM },
    { "charstatus", "CHARACTER", "charstatus", 0x1ea, 0x2 },
    { "nm", "MONSTER", "nomonster", 0x1ec, ARG_TYPE_NO_PARAM },
    { "m", "MONSTER", "monsterclass", 0x1ed, 0x4 },
    { "minfo", "MONSTER", "monsterinfo", 0x1f1, ARG_TYPE_NO_PARAM },
    { "md", "MONSTER", "monsterdebug", 0x1f2, 0x4 },
    { "rare", "ITEM", "rare", 0x1f6, ARG_TYPE_NO_PARAM },
    { "unique", "ITEM", "unique", 0x1f7, ARG_TYPE_NO_PARAM },
    { "act", "INTERFACE", "act", 0x1fb, 0x4 },
    { "log", "DEBUGS", "log", 0x212, ARG_TYPE_NO_PARAM },
    { "msglog", "DEBUGS", "msglog", 0x212, ARG_TYPE_NO_PARAM },
    { "safe", "DEBUGS", "safemode", 0x213, ARG_TYPE_NO_PARAM },
    { "nosave", "DEBUGS", "nosave", 0x215, ARG_TYPE_NO_PARAM },
    { "seed", "DEBUGS", "seed", 0x216, 0x4 },
    { "teen", "DEBUGS", "teen", 0x21a, ARG_TYPE_NO_PARAM },
    { "cheats", "DEBUGS", "cheats", 0x219, ARG_TYPE_NO_PARAM },
    { "ns", "DEBUGS", "nosound", 0x21b, ARG_TYPE_NO_PARAM },
    { "questall", "DEBUGS", "questall", 0x21c, ARG_TYPE_NO_PARAM },
    { "build", "DEBUGS", "build", 0x21e, ARG_TYPE_NO_PARAM },
    { "npl", "FILEIO", "nopreload", 0x1fe, ARG_TYPE_NO_PARAM },
    { "direct", "FILEIO", "direct", 0x1ff, ARG_TYPE_NO_PARAM },
    { "lem", "FILEIO", "lowend", 0x200, ARG_TYPE_NO_PARAM },
    { "nocompress", "FILEIO", "nocompress", 0x202, ARG_TYPE_NO_PARAM },
    { "comint", "BNET", "comint", 0x221, 0x4 },
    { "token", "BNET", "token", 0x223, 0x4 },
    { "gamepass", "BNET", "gamepass", 0x23f, 0x18 },
    { "skiptobnet", "BNET", "skiptobnet", 0x359, ARG_TYPE_NO_PARAM },
    { "client", "CUSTOM", "client", 0x79b, ARG_TYPE_NO_PARAM },
    { "server", "CUSTOM", "server", 0x79c, ARG_TYPE_NO_PARAM },
    { "launch", "CUSTOM", "launch", 0x79d, ARG_TYPE_NO_PARAM },
    { "multi", "CUSTOM", "multi", 0x79e, ARG_TYPE_NO_PARAM },
    { "notitle", "CUSTOM", "notitle", 0x79f, ARG_TYPE_NO_PARAM },
    { "res800", "CUSTOM", "res800", 0x7a0, ARG_TYPE_NO_PARAM },
    { "res640", "CUSTOM", "res640", 0x7a1, ARG_TYPE_NO_PARAM },
    { "nonotify", "CUSTOM", "nonotify", 0x7a2, ARG_TYPE_NO_PARAM },
    { "noexit", "CUSTOM", "noexit", 0x7a3, ARG_TYPE_NO_PARAM },
    { "autorest", "CUSTOM", "autorestart", 0x7a4, ARG_TYPE_NO_PARAM },
    { "nohook", "CUSTOM", "nohook", 0x7a5, ARG_TYPE_NO_PARAM },
    { "nochar", "CUSTOM", "nochar", 0x7a6, ARG_TYPE_NO_PARAM },
    { "clientexit", "CUSTOM", "clientexit", 0x7a7, ARG_TYPE_NO_PARAM },
    { "multiclient", "CUSTOM", "multiclient", 0x7a8, ARG_TYPE_NO_PARAM },
    { "printarg", "CUSTOM", "printarg", 0x7a9, ARG_TYPE_NO_PARAM },
    { "noscript", "CUSTOM", "noscript", 0x7aa, ARG_TYPE_NO_PARAM },
    { "noplugin", "CUSTOM", "noplugin", 0x7ab, ARG_TYPE_NO_PARAM },
    { "l", "CUSTOM", "debuglog", 0x7ac, ARG_TYPE_NO_PARAM },
    { "console", "CUSTOM", "console", 0x7ad, ARG_TYPE_NO_PARAM },
    { "rmconffile", "CUSTOM", "rmconffile", 0x7ae, ARG_TYPE_NO_PARAM },
    { "nocleanup", "CUSTOM", "nocleanup", 0x7af, ARG_TYPE_NO_PARAM },
    { "servername", "CUSTOM", "servername", 0x7ec, 0x18 },
    { "pri", "CUSTOM", "priority", 0x7bc, 0x18 },
    { "title", "CUSTOM", "title", 0x804, 0x80 },
    { "locale", "CUSTOM", "locale", 0x7d4, 0x18 },
    { "cfg", "CUSTOM", "conffile", 0x884, 0x104 },
    { "mpq", "CUSTOM", "mpqfile", 0x988, 0x104 },
    { "pdir", "CUSTOM", "plugindir", 0xa8c, 0x104 },
    { "sfile", "CUSTOM", "scriptfile", 0xb90, 0x104 },
    { "test", "CUSTOM", "test", 0x7b8, 0x4 },
    { NULL, NULL, NULL, 0, 0 },
};

/* function prototype */

/* struct */

// 在这里使用union简直绝妙！不用费劲心思凑字段的偏移量了。
#pragma pack(1) // 逐字节对齐，方便定位
union version_info
{
    #pragma pack(1)
    struct
    {
        // 不允许使用不完整的类型
        // BYTE offset[0];
        BYTE value;
    } db_0000_expansion;

    #pragma pack(1)
    struct
    {
        BYTE offset[4];
        char value[5 + 1];
    } db_0004_str;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x20d];
        BOOL (*value)();
    } dd_020d_IsExpansion;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x79d];
        BYTE value;
    } db_079d;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x7ac];
        BYTE value;
    } db_07ac_enableLogFile;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x7af];
        BYTE value;
    } db_07af;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x7b4];
        DWORD value;
    } dw_07b4_gameProductVersionFlag;

    // 进程的优先级。默认为 bnormal，即 低于正常。
    #pragma pack(1)
    struct
    {
        BYTE offset[0x7bc];
        char value[7 + 1];
    } db_07bc_processPriority;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x7ec];
        char value[9 + 1];
    } db_07ec_gameName;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x804];
        char value[38 + 1];
    } db_0804_title;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x884];
        char value;
    } db_0884_filename;

    BYTE padding[0xc94];
};

void sub_404ed0_LogFormat(const char* tag, const char* format, ...)
{
    time_t now;
    char timestamp[32];
    time(&now);
    struct tm* localNow = localtime(&now);
    if (localNow)
    {
        strftime(timestamp, sizeof(timestamp), "%b %d %H:%M:%S", localNow);
    }
    else
    {
        strcpy(timestamp, "?");
    }

    if (global_dd_408590_logFlag & LOG_TYPE_FILE)
    {
        if (global_dd_40858c_logFile)
        {
            fprintf(global_dd_40858c_logFile, "%s %s: ", timestamp, tag);

            va_list ap;
            va_start(ap, format);
            vfprintf(global_dd_40858c_logFile, format, ap);
            va_end(ap);

            fprintf(global_dd_40858c_logFile, "\n");
            fflush(global_dd_40858c_logFile);
        }
    }

    if (global_dd_408590_logFlag & LOG_TYPE_CONSOLE)
    {
        // 原始汇编是向 _iob + 0x20 写入日志。
        // 最新的MSVC没有 _iob 了。干脆改为 stdout 吧。
        FILE* consoleFilePtr = stdout;

        fprintf(consoleFilePtr, "%s %s: ", timestamp, tag);

        va_list ap;
        va_start(ap, format);
        vfprintf(consoleFilePtr, format, ap);
        va_end(ap);

        fprintf(consoleFilePtr, "\n");
        fflush(consoleFilePtr);
    }
}

BOOL sub_407380_CheckFileExist(const char* filePath)
{
    HANDLE file = CreateFileA(
        filePath,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);
    BOOL exist = (file != INVALID_HANDLE_VALUE);
    CloseHandle(file);
    return exist;
}

BOOL sub_40735e_CheckExpansion()
{
    if (!sub_407380_CheckFileExist(CSTR_D2_EXP_DOT_MPQ))
    {
        LogFormat("%s not exist.\n", CSTR_D2_EXP_DOT_MPQ);
        return FALSE;
    }
    if (!sub_407380_CheckFileExist(CSTR_D2_X_TALK_DOT_MPQ))
    {
        LogFormat("%s not exist.\n", CSTR_D2_X_TALK_DOT_MPQ);
        return FALSE;
    }
    /* 这里的汇编代码比较难懂：
    * push filePath
    * call CheckFileExist
    * neg eax
    * sbb eax, eax
    * pop ecx ;__cdecl 调用约定调用方来平衡栈，所以会 add esp, xxx 或者用若干个 pop ecx的方式来平衡栈。
    * neg eax
    * 
    * neg eax 的意思是 eax <- (0 - eax)，当eax非0时，CF标志位置位。
    * sbb是带借位的减法，即 eax <- (eax - eax - CF)。
    * 可以从eax是否为0，还是非0来分别讨论。
    */
    return TRUE;
}

BOOL sub_406bab_IsExpansion()
{
    if (global_dd_408620_Dst == NULL)
    {
        return FALSE;
    }
    return global_dd_408620_Dst->db_0000_expansion.value;
}

void sub_4069d8_InitializeDefaultSettings()
{
    memset(global_dd_408620_Dst, 0, sizeof(union version_info));
    if (sub_40735e_CheckExpansion())
    {
        global_dd_408620_Dst->db_0000_expansion.value = TRUE;
    }

    global_dd_408620_Dst->dd_020d_IsExpansion.value = sub_406bab_IsExpansion;
    global_dd_408620_Dst->db_079d.value = TRUE;
    global_dd_408620_Dst->db_07af.value = TRUE;
    strcpy(global_dd_408620_Dst->db_0804_title.value, CSTR_D2_LOADER_VERSION_AND_BUILD);
    strcpy(global_dd_408620_Dst->db_07ec_gameName.value, CSTR_DIABLO_II);
    // 怪不得以前打开windows的任务管理器查看d2loader.exe的优先级，总是显示为低于正常。
    strcpy(global_dd_408620_Dst->db_07bc_processPriority.value, CSTR_PROCESS_PRIORITY_BELOW_NORMAL);
}

/**
 * 切出 args 中的第一个命令行参数，存入 buffer 中。
 * @param args 非空格开头的命令行参数字符串
 * @param buffer 存储结果的缓冲区
 * @returns 返回下一次处理时的起始位置
 * */
const char* sub_406a68_CutFirstArgument(const char* args, char* buffer)
{
    if (args == NULL)
    {
        return NULL;
    }

    const char* eax_p = args;
    char* edx_pBuffer = buffer;
    BOOL esi_isDoubleQuoteMode = FALSE;

    // 在汇编里while循环被拆出一个单独的跳转，*eax_p != '\0' 对应的代码被重复了一遍。
    while (*eax_p != '\0')
    {
        if (*eax_p == '"')
        {
            /* xor ecx, ecx
             * test esi, esi
             * setz cl
             * mov esi, ecx
             * setz 的意思是如果ZF标志位为1，则将cl置为1，否则置为0；
             * 也就是如果 esi 为0，则esi变为1；否则esi变为0；相当于取非运算；
             * 避免了条件跳转。
             * */
            esi_isDoubleQuoteMode = !esi_isDoubleQuoteMode;
        }
        else
        {
            if (*eax_p == ' ' && !esi_isDoubleQuoteMode)
            {
                break;
            }

            // 命令行中的 \" 会作为字符串内容对待，而不是作为含有空格的字符串边界定界符对待。
            if (*eax_p == '\\' && eax_p[1] == '"')
            {
                eax_p = eax_p + 1;
            }

            // 拷贝这个有效内容
            *edx_pBuffer = *eax_p;
            edx_pBuffer++;
        }
        eax_p++;
    }

    *edx_pBuffer = '\0';
    
    // 移除命令行参数之间用于分隔的空格字符
    while (*eax_p == ' ')
    {
        eax_p++;
    }

    return eax_p;
}

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
    if (global_dd_402ea8_indexTable[0].category == (void*)ebp_index)
    {
        return -1;
    }

    // 指向去除“-”的参数名
    const char* ebx_argName = buffer + 1;

    struct string_index_item* esi_stringTableEntry = &global_dd_402ea8_indexTable[0]; //&global_dd_402eb0;
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

    return ebp_index;
}

void sub_406b12(int i, char* buffer)
{

}

BOOL sub_406887(char* commandLine)
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
            if (global_dd_402ea8_indexTable[i].type != ARG_TYPE_NO_PARAM)
            {
                edi_s = sub_406a68_CutFirstArgument(edi_s, buffer);
            }

            sub_406b12(i, buffer);
        }
    } while (*edi_s != '\0');
    
    return TRUE;
}

BOOL sub_4068f2(char* filename)
{
    //TODO
}

BOOL sub_406bb9()
{
    //TODO
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

BOOL sub_406803_InitializeSettings()
{
    assert(sizeof(union version_info) == 0xc94);
    global_dd_408620_Dst = malloc(sizeof(union version_info));
    if (!global_dd_408620_Dst)
    {
        return FALSE;
    }
    sub_4069d8_InitializeDefaultSettings();
    char* commandLine = GetCommandLineA();
    if (!sub_406887(commandLine))
    {
        return FALSE;
    }
    sub_4068f2(&global_dd_408620_Dst->db_0884_filename.value);

    char* dstStr = global_dd_408620_Dst->db_0004_str.value;
    if (dstStr[0] == 0 &&
        dstStr[1] == 0 &&
        dstStr[2] == 0 &&
        dstStr[3] == 0 &&
        dstStr[4] == 0)
    {
        sub_406bb9();
    }

    char* priority = global_dd_408620_Dst->db_07bc_processPriority.value;
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
    global_dd_408620_Dst->dw_07b4_gameProductVersionFlag.value = esi;
}

BOOL sub_404c57()
{
    //TODO
}

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd)
{
    InitLogFile();

    if (!sub_406803_InitializeSettings())
    {
        return 0;
    }

    if (global_dd_408620_Dst->db_07ac_enableLogFile.value)
    {
        global_dd_408588_logFile = fopen(CSTR_D2_LOADER_DOT_LOG, "a");
        sub_404eb1_SetLogFile(global_dd_408588_logFile);
        sub_404ec5_EnableLog(LOG_TYPE_FILE);
        // add esp, 10h 是平衡前面的三个C函数调用造成的栈变化
    }

    sub_406c59_CheckGameExeVersion();
    sub_404ed0_LogFormat(LOG_TAG(WinMain), CSTR_LOG_START_SEPARATOR_LINE);
    sub_404ed0_LogFormat(LOG_TAG(WinMain), CSTR_D2_LOADER_AUTHOR_STATEMENT);
    char* commandLine = GetCommandLineA();
    sub_404ed0_LogFormat(LOG_TAG(WinMain), "Command Line: %s", commandLine);
    sub_404ed0_LogFormat(LOG_TAG(WinMain), "Loader Version: %s", CSTR_D2_LOADER_VERSION_AND_BUILD);
    sub_404ed0_LogFormat(LOG_TAG(WinMain), "Client Version: 0x%08X", global_dd_408620_Dst->dw_07b4_gameProductVersionFlag.value);
    const char* mode = global_dd_408620_Dst->db_0000_expansion.value ? "Expansion" : "Classic";
    sub_404ed0_LogFormat(LOG_TAG(WinMain), "Running in %s Mode", mode);
    
    sub_404c57();

    sub_404eb1_SetLogFile(NULL);

    if (global_dd_408588_logFile)
    {
        fclose(global_dd_408588_logFile);
    }

    return 1;
}