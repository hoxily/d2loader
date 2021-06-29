#include <Windows.h>
#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include "logger.h"
#include "constant-strings.h"

/*
 * ��������ǰ׺��
 * db: 1 �ֽ�
 * dw: 2 �ֽ�
 * dd: 4 �ֽ�
 */

/* global variable */
union program_setting_store* global_dd_408620_settings;
FILE* global_dd_408588_logFile;
FILE* global_dd_40858c_logFile;

#define LOG_TYPE_FILE 0x20
#define LOG_TYPE_CONSOLE 0x10

DWORD global_dd_408590_logFlag;
char global_db_402958_pluginDir[0x104] = "Plugin";
DWORD global_dd_4085f0;
CRITICAL_SECTION global_dd_4085f8_criticalSection;

struct loaded_plugin_item
{
    DWORD dd_0000_activeFlag;
    HMODULE dd_0004_dllModule;
    HANDLE dd_0008_autoResetEventHandle;
    char* dd_000c_dllFileName;
    struct query_interface_result* dd_0010_queryInterfaceResult;
};

struct loaded_plugin_item* global_dd_408610_plugins;
DWORD global_dd_408614_count;
int global_dd_408618_activePluginCount;
DWORD global_dd_40861c_capacity;

#define RUN_PLUGIN_REASON_INIT 0x1ul
#define RUN_PLUGIN_REASON_CLEANUP 0x2ul
#define RUN_PLUGIN_REASON_ENTER_GAME 0x4ul
#define RUN_PLUGIN_REASON_LEAVE_GAME 0x8ul
#define RUN_PLUGIN_REASON_ENTER_CHANNEL 0x10ul
#define RUN_PLUGIN_REASON_LEAVE_CHANNEL 0x20ul
#define RUN_PLUGIN_REASON_ENTER_MAIN_MENU 0x40ul
#define RUN_PLUGIN_REASON_LEAVE_MAIN_MENU 0x80ul
#define RUN_PLUGIN_REASON_ERROR_CLEANUP 0x100ul

// �������в����޾������ֵ
#define ARG_TYPE_NO_PARAM 1
// �������в���Ϊ16λ�޷���������֧��ʮ���ơ��˽��ơ�ʮ�����Ƶȶ��ֱ�﷽ʽ
#define ARG_TYPE_UNSIGNED_SHORT_INT 2
// �������в���Ϊ32λ�޷���������֧��ʮ���ơ��˽��ơ�ʮ�����Ƶȶ��ֱ�﷽ʽ
#define ARG_TYPE_UNSIGNED_INT 4
// ������ֵ��ʾ�ò���ֵ��Ϊ�ַ����Ĵ洢�������Ĵ�С

#pragma pack(1)
struct string_index_item
{
    // �����Ķ���
    const char* shortName;
    // ��������
    const char* category;
    // �����ĳ���
    const char* longName;
    // �ò����� program_setting_store �ṹ�е�ƫ����
    DWORD offset;
    // ����������
    DWORD type;
};

struct string_index_item global_dd_402ea8_CommandLineArgumentTable[93] = {
    { "exp", "MAIN", "expansion", 0x0, ARG_TYPE_NO_PARAM },
    { "w", "VIDEO", "window", 0x4, ARG_TYPE_NO_PARAM },
    { "glide", "VIDEO", "glide", 0x6, ARG_TYPE_NO_PARAM },
    { "opengl", "VIDEO", "opengl", 0x7, ARG_TYPE_NO_PARAM },
    { "d3d", "VIDEO", "d3d", 0x9, ARG_TYPE_NO_PARAM },
    { "rave", "VIDEO", "rave", 0x8, ARG_TYPE_NO_PARAM },
    { "per", "VIDEO", "perspective", 0x9, ARG_TYPE_NO_PARAM },
    { "lq", "VIDEO", "lowquality", 0xa, ARG_TYPE_NO_PARAM },
    { "gamma", "VIDEO", "gamma", 0xc, ARG_TYPE_UNSIGNED_INT },
    { "vsync", "VIDEO", "vsync", 0x10, ARG_TYPE_NO_PARAM },
    { "fr", "VIDEO", "framerate", 0x10, ARG_TYPE_UNSIGNED_INT },
    { "s", "NETWORK", "serverip", 0x33, 0x18 },
    { "gametype", "NETWORK", "gametype", 0x14, ARG_TYPE_UNSIGNED_INT },
    { "joinid", "NETWORK", "joinid", 0x18, ARG_TYPE_UNSIGNED_SHORT_INT },
    { "gamename", "NETWORK", "gamename", 0x1a, 0x18 },
    { "bn", "NETWORK", "battlenetip", 0x4b, 0x18 },
    { "mcpip", "NETWORK", "mcpip", 0x63, 0x18 },
    { "nopk", "NETWORK", "nopk", 0x7e, ARG_TYPE_NO_PARAM },
    { "openc", "NETWORK", "openc", 0x7f, ARG_TYPE_NO_PARAM },
    { "arena", "GAME", "arena", 0x202, ARG_TYPE_UNSIGNED_SHORT_INT },
    { "difficulty", "GAME", "difficulty", 0x20b, ARG_TYPE_NO_PARAM },
    { "txt", "GAME", "text", 0x211, ARG_TYPE_NO_PARAM },
    // -ama �� -pal ��ƫ������һ���ġ������� d2loader ��һ��bug��
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
    { "charstatus", "CHARACTER", "charstatus", 0x1ea, ARG_TYPE_UNSIGNED_SHORT_INT },
    { "nm", "MONSTER", "nomonster", 0x1ec, ARG_TYPE_NO_PARAM },
    { "m", "MONSTER", "monsterclass", 0x1ed, ARG_TYPE_UNSIGNED_INT },
    { "minfo", "MONSTER", "monsterinfo", 0x1f1, ARG_TYPE_NO_PARAM },
    { "md", "MONSTER", "monsterdebug", 0x1f2, ARG_TYPE_UNSIGNED_INT },
    { "rare", "ITEM", "rare", 0x1f6, ARG_TYPE_NO_PARAM },
    { "unique", "ITEM", "unique", 0x1f7, ARG_TYPE_NO_PARAM },
    { "act", "INTERFACE", "act", 0x1fb, ARG_TYPE_UNSIGNED_INT },
    { "log", "DEBUGS", "log", 0x212, ARG_TYPE_NO_PARAM },
    { "msglog", "DEBUGS", "msglog", 0x212, ARG_TYPE_NO_PARAM },
    { "safe", "DEBUGS", "safemode", 0x213, ARG_TYPE_NO_PARAM },
    { "nosave", "DEBUGS", "nosave", 0x215, ARG_TYPE_NO_PARAM },
    { "seed", "DEBUGS", "seed", 0x216, ARG_TYPE_UNSIGNED_INT },
    { "teen", "DEBUGS", "teen", 0x21a, ARG_TYPE_NO_PARAM },
    { "cheats", "DEBUGS", "cheats", 0x219, ARG_TYPE_NO_PARAM },
    { "ns", "DEBUGS", "nosound", 0x21b, ARG_TYPE_NO_PARAM },
    { "questall", "DEBUGS", "questall", 0x21c, ARG_TYPE_NO_PARAM },
    { "build", "DEBUGS", "build", 0x21e, ARG_TYPE_NO_PARAM },
    { "npl", "FILEIO", "nopreload", 0x1fe, ARG_TYPE_NO_PARAM },
    { "direct", "FILEIO", "direct", 0x1ff, ARG_TYPE_NO_PARAM },
    { "lem", "FILEIO", "lowend", 0x200, ARG_TYPE_NO_PARAM },
    { "nocompress", "FILEIO", "nocompress", 0x202, ARG_TYPE_NO_PARAM },
    { "comint", "BNET", "comint", 0x221, ARG_TYPE_UNSIGNED_INT },
    { "token", "BNET", "token", 0x223, ARG_TYPE_UNSIGNED_INT },
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
    { "test", "CUSTOM", "test", 0x7b8, ARG_TYPE_UNSIGNED_INT },
    { NULL, NULL, NULL, 0, 0 },
};

/* function prototype */

/* struct */

// ������ʹ��union��ֱ������÷Ѿ���˼���ֶε�ƫ�����ˡ�
#pragma pack(1) // ���ֽڶ��룬���㶨λ
union program_setting_store
{
    #pragma pack(1)
    struct
    {
        // ������ʹ�ò�����������
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
        BYTE offset[0x7a9];
        BYTE value;
    } db_07a9_printArg;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x7aa];
        BYTE value;
    } db_07aa_noScript;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x7ab];
        BYTE value;
    } db_07ab_noPlugin;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x7ac];
        BYTE value;
    } db_07ac_enableLogFile;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x7ae];
        BYTE value;
    } db_07ae_removeConfFile;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x7af];
        BYTE value;
    } db_07af_noCleanup;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x7b4];
        DWORD value;
    } dd_07b4_gameProductVersionFlag;

    // ���̵����ȼ���Ĭ��Ϊ bnormal���� ����������
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
        char value[0x104];
    } db_0884_confFile;

    #pragma pack(1)
    struct
    {
        BYTE offset[0xa8c];
        char value[0x104];
    } db_0a8c_pluginDir;

    BYTE padding[0xc94];
};

// fn_OnInitPlugin ������ջƽ���Ǻ����ڲ����ġ�
typedef void (__stdcall *fn_OnInitPlugin)(DWORD gameProductVersionFlag);
// fn_RunPlugin ������ջƽ���Ǻ����ڲ����ġ�
typedef BOOL (__stdcall* fn_RunPlugin)(DWORD reasonFlag, DWORD* gameProductVersionFlag);

struct query_interface_result
{
    // 0x44320000
    DWORD magic;
    // 0x01000912
    DWORD version;
    const char* pluginName;
    fn_RunPlugin runPlugin;
};

struct query_interface_result_old
{
    // 0x44320000
    DWORD magic;
    const char* pluginName;
    fn_OnInitPlugin init;
};

// ���ڲ������������� __stdcall �� __cdecl һ�¡�
typedef struct query_interface_result* (__stdcall *fn_QueryInterface)();

typedef HMODULE (WINAPI *fn_LoadLibraryA)(_In_ LPCSTR lpLibFileName);
fn_LoadLibraryA global_dd_4085a0_LoadLibraryA;

typedef DWORD (WINAPI *fn_GetModuleFileNameA)(
    _In_opt_ HMODULE hModule,
    _Out_ LPSTR lpFilename,
    _In_ DWORD nSize);
fn_GetModuleFileNameA global_dd_4085a4_GetModuleFileNameA;

typedef HWND (WINAPI *fn_FindWindowA)(
    _In_opt_ LPCSTR lpClassName,
    _In_opt_ LPCSTR lpWindowName
    );
fn_FindWindowA global_dd_4085a8_FindWindowA;

const char* global_dd_408598_moduleD2LoaderFileName;
const char* global_dd_40859c_gameDotExeFileName;
HMODULE global_dd_4085b0_moduleBnClient;
HMODULE global_dd_4085ac_moduleD2Gfx;
HMODULE global_dd_4085b4_moduleD2Net;

struct hook_search_item
{
    const char* functionName;
    void* functionProcAddress;
};

DWORD WINAPI sub_40543b_my_GetModuleFileNameA(HMODULE hModule, LPSTR lpFilename, DWORD nSize)
{
    DWORD len;
    if (global_dd_4085a4_GetModuleFileNameA == NULL)
    {
        len = GetModuleFileNameA(hModule, lpFilename, nSize);
    }
    else
    {
        len = global_dd_4085a4_GetModuleFileNameA(hModule, lpFilename, nSize);
    }

    if (global_dd_408598_moduleD2LoaderFileName == NULL)
    {
        return len;
    }

    if (global_dd_40859c_gameDotExeFileName == NULL)
    {
        return len;
    }

    if (len == 0)
    {
        return len;
    }

    // ���д��볢�Ի�ȡ����ģ���������� d2loader.exe ʱ���滻�� game.exe ���������Ǿ�
    // ��֪���Լ���hook�ˡ�
    if (lstrcmpiA(lpFilename, global_dd_408598_moduleD2LoaderFileName) != 0)
    {
        return len;
    }

    (void)lstrcpynA(lpFilename, global_dd_40859c_gameDotExeFileName, nSize);
    lpFilename[nSize - 1] = '\0';
    /*
    * call esi ; esi is lstrlenA
    * cmp eax, edi ; edi is nSize - 1
    * jnb short loc_4054b7
    * 
    * �����jnb��ָjump short if not below (CF=0)
    */
    if ((DWORD)lstrlenA(global_dd_40859c_gameDotExeFileName) >= nSize - 1)
    {
        return nSize - 1;
    }
    else
    {
        return lstrlenA(global_dd_40859c_gameDotExeFileName);
    }
}

HWND WINAPI sub_4053fd_my_FindWindowA(LPCSTR lpClassName, LPCSTR lpWindowName)
{
    //TODO
}

HMODULE WINAPI sub_4054c2_my_LoadLibraryA(LPCSTR lpLibFileName);

struct hook_search_item global_dd_4023f0[3] =
{
    { "LoadLibraryA", sub_4054c2_my_LoadLibraryA },
    { "GetModuleFileNameA", sub_40543b_my_GetModuleFileNameA },
    { (const char*)-1, NULL }
};

BOOL sub_40513a(
    HMODULE hModule,
    const char* hookDll,
    struct hook_search_item* functionNameList,
    void* null1,
    void* null2);

HMODULE WINAPI sub_4054c2_my_LoadLibraryA(LPCSTR lpLibFileName)
{
    HMODULE hModule = NULL;
    if (global_dd_4085a0_LoadLibraryA == NULL)
    {
        hModule = LoadLibraryA(lpLibFileName);
    }
    else
    {
        hModule = global_dd_4085a0_LoadLibraryA(lpLibFileName);
    }

    if (hModule != NULL)
    {
        sub_40513a(
            hModule,
            "Kernel32.dll",
            global_dd_4023f0,
            NULL,
            NULL
        );
    }

    return hModule;
}

struct hook_search_item global_dd_402408[2] =
{
    { "FindWindowA", sub_4053fd_my_FindWindowA },
    { (const char*)-1, NULL }
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
        // ԭʼ������� _iob + 0x20 д����־��
        // ���µ�MSVCû�� _iob �ˡ��ɴ��Ϊ stdout �ɡ�
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
    /* ����Ļ�����Ƚ��Ѷ���
    * push filePath
    * call CheckFileExist
    * neg eax
    * sbb eax, eax
    * pop ecx ;__cdecl ����Լ�����÷���ƽ��ջ�����Ի� add esp, xxx ���������ɸ� pop ecx�ķ�ʽ��ƽ��ջ��
    * neg eax
    * 
    * neg eax ����˼�� eax <- (0 - eax)����eax��0ʱ��CF��־λ��λ��
    * sbb�Ǵ���λ�ļ������� eax <- (eax - eax - CF)��
    * ���Դ�eax�Ƿ�Ϊ0�����Ƿ�0���ֱ����ۡ�
    */
    return TRUE;
}

BOOL sub_406bab_IsExpansion()
{
    if (global_dd_408620_settings == NULL)
    {
        return FALSE;
    }
    return global_dd_408620_settings->db_0000_expansion.value;
}

void sub_4069d8_InitializeDefaultSettings()
{
    memset(global_dd_408620_settings, 0, sizeof(union program_setting_store));
    if (sub_40735e_CheckExpansion())
    {
        global_dd_408620_settings->db_0000_expansion.value = TRUE;
    }

    global_dd_408620_settings->dd_020d_IsExpansion.value = sub_406bab_IsExpansion;
    global_dd_408620_settings->db_079d.value = TRUE;
    global_dd_408620_settings->db_07af_noCleanup.value = TRUE;
    strcpy(global_dd_408620_settings->db_0804_title.value, CSTR_D2_LOADER_VERSION_AND_BUILD);
    strcpy(global_dd_408620_settings->db_07ec_gameName.value, CSTR_DIABLO_II);
    // �ֲ�����ǰ��windows������������鿴d2loader.exe�����ȼ���������ʾΪ����������
    strcpy(global_dd_408620_settings->db_07bc_processPriority.value, CSTR_PROCESS_PRIORITY_BELOW_NORMAL);
}

/**
 * �г� args �еĵ�һ�������в��������� buffer �С�
 * @param args �ǿո�ͷ�������в����ַ���
 * @param buffer �洢����Ļ�����
 * @returns ������һ�δ���ʱ����ʼλ��
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

    // �ڻ����whileѭ�������һ����������ת��*eax_p != '\0' ��Ӧ�Ĵ��뱻�ظ���һ�顣
    while (*eax_p != '\0')
    {
        if (*eax_p == '"')
        {
            /* xor ecx, ecx
             * test esi, esi
             * setz cl
             * mov esi, ecx
             * setz ����˼�����ZF��־λΪ1����cl��Ϊ1��������Ϊ0��
             * Ҳ������� esi Ϊ0����esi��Ϊ1������esi��Ϊ0���൱��ȡ�����㣻
             * ������������ת��
             * */
            esi_isDoubleQuoteMode = !esi_isDoubleQuoteMode;
        }
        else
        {
            if (*eax_p == ' ' && !esi_isDoubleQuoteMode)
            {
                break;
            }

            // �������е� \" ����Ϊ�ַ������ݶԴ�����������Ϊ���пո���ַ����߽綨����Դ���
            if (*eax_p == '\\' && eax_p[1] == '"')
            {
                eax_p = eax_p + 1;
            }

            // ���������Ч����
            *edx_pBuffer = *eax_p;
            edx_pBuffer++;
        }
        eax_p++;
    }

    *edx_pBuffer = '\0';
    
    // �Ƴ������в���֮�����ڷָ��Ŀո��ַ�
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
    if (global_dd_402ea8_CommandLineArgumentTable[0].category == (void*)ebp_index)
    {
        return -1;
    }

    // ָ��ȥ����-���Ĳ�����
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

    // hoxily fix: ���û���ҵ����ʵĲ��������򷵻� -1��
    if (esi_stringTableEntry[0].category == NULL)
    {
        return -1;
    }
    else
    {
        return ebp_index;
    }
}

// �洢һ�������в���
// @param i �����в���������������±�
// @param buffer �������в�����ֵ
BOOL sub_406b12_StoreCommandLineSetting(int i, char* buffer)
{
    assert(offsetof(struct string_index_item, type) == 0x10);
    DWORD type = global_dd_402ea8_CommandLineArgumentTable[i].type;
    DWORD offset = global_dd_402ea8_CommandLineArgumentTable[i].offset;
    if (type == ARG_TYPE_NO_PARAM)
    {
        BYTE toggle = global_dd_408620_settings->padding[offset];
        // ԭ������ xor byte ptr [eax], 1
        // �������ᷭתԭ���Ѿ����úõ������
        // Ԥ�⣺d2loader.exe -w -w  ���������������������ᴰ�ڻ����У�����ȫ�����С�
        // ʵ������ȷʵ��û�д��ڻ����С�
        global_dd_408620_settings->padding[offset] = !toggle;
    }
    else if (type == ARG_TYPE_UNSIGNED_SHORT_INT)
    {
        /*
        * ��� base(_Radix) Ϊ0����ʹ�� strSource ָ����ַ����ĳ�ʼ�ַ���ȷ������
        * �����һ���ַ�Ϊ 0���ҵڶ����ַ���Ϊ��x����X�����򽫸��ַ�����Ϊ�˽���������
        * �����һ���ַ�Ϊ��0�����ҵڶ����ַ�Ϊ��x����X�����򽫸��ַ�����Ϊʮ������������
        * �����һ���ַ��ǡ�1������9�����򽫸��ַ�����Ϊʮ����������
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
        // ��ʱ type ��ʾ�ַ����������Ĵ�С��
        int settingBufferSize = type;
        // ʹ�� (void) ���� lstrcpynA �ķ��ؽ��������Visual Studio����warning��
        (void)lstrcpynA(global_dd_408620_settings->padding + offset, buffer, settingBufferSize);
        // ǿ�����û����������һ���ַ�Ϊnull�����������������
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
        // ��������ϱ�ʾ���esi�Ƿ�Ϊ������
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
    // Buf �� var_27FF ������ͬһ��������var_27FF �ȼ��� Buf + 1
    char buffer[0x2800];

    // call __alloca_probe ��������ջ�������˴��� PAGE_SIZE ��С�Ŀռ䣬���±�������������μ����롣
    // ����ȷ���������е�����ʱ��ջ�Ŀռ��㹻������� https://bbs.pediy.com/thread-145752.htm
    // �˴����Ǻ�������
    
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

    // fgets(buffer, edi_bufferSize, ebx_fp) �����ڻ���ﱻ��ֳ���������
    while (fgets(buffer, edi_bufferSize, ebx_fp))
    {
        // fgets �Ὣ \n �ַ�Ҳ���� buffer �С�
        int eax_i = strlen(buffer);
        // dec eax ָ�� ��Ӱ�� CF ��־λ������ OF, SF, ZF, AF, PF ��λ������ݼ�������Ӧ���á�
        // js ָ���ʾjump short if sign (SF=1)��
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

BOOL sub_404c57_GameMain()
{
    if (global_dd_408620_settings->db_07a9_printArg.value)
    {
        sub_404ed0_LogFormat(
            LOG_TAG(sub_404c57_GameMain),
            "Printing Parameters Table");
        sub_4066dc_PrintParametersTable(global_dd_408588_logFile);
        sub_404ed0_LogFormat(
            LOG_TAG(sub_404c57_GameMain),
            "Parameters Table Printed, Exitting");
    }
    else
    {
        if (global_dd_408620_settings->db_07ab_noPlugin.value)
        {
            sub_404ed0_LogFormat(
                LOG_TAG(sub_404c57_GameMain),
                "Plugin Have Been Disabled");
        }
        else
        {
            sub_406014_PluginInit();
        }

        BOOL hookRet = sub_4054fd_HookDll();
        sub_404ed0_LogFormat(
            LOG_TAG(sub_404c57_GameMain),
            "Hook Returned %s",
            hookRet ? "True" : "False");

        if (!sub_40a480())
        {
            sub_404ed0_LogFormat(
                LOG_TAG(sub_404c57_GameMain),
                "Game Initialize Failed, Exitting\n");
            return FALSE;
        }

        if (!global_dd_408620_settings->db_07aa_noScript.value)
        {
            sub_4057a8();
        }

        sub_404ed0_LogFormat(
            LOG_TAG(sub_404c57_GameMain),
            "Entering Game Client Loop");
        sub_404d77_GameClientLoop();
        sub_404ed0_LogFormat(
            LOG_TAG(sub_404c57_GameMain),
            "Exit From Game Client Loop, Cleanup");
        sub_40a600_Cleanup();
        // �����һ�� LogFormat ���ã�����û���ṩ�κβ���������֡�
        // call sub_404ed0
        // ����call sub_40a600_Cleanup ֮ǰ������LogFormat���ý�����û��ƽ��ջ��
        // ���������ٴε��� LogFormat���൱�������ϴ�push�Ĳ�����
        sub_404ed0_LogFormat(
            LOG_TAG(sub_404c57_GameMain),
            "Exit From Game Client Loop, Cleanup");
        // ʵ�ⷢ�֣���û��������� ��Exit From Game Client Loop, Cleanup��
        
        if (global_dd_408620_settings->db_07af_noCleanup.value)
        {
            ExitProcess(1);
            assert("Unreachable code.");
        }
        else
        {
            sub_4071a5();
            sub_405af1();
            sub_406175();
            sub_405602();
            sub_4069bc();
            sub_404ed0_LogFormat(
                LOG_TAG(sub_404c57_GameMain),
                "Cleanup Done, Exitting\n");
        }
    }
    
    return TRUE;
    
}

// sub_404b60
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

    if (global_dd_408620_settings->db_07ac_enableLogFile.value)
    {
        global_dd_408588_logFile = fopen(CSTR_D2_LOADER_DOT_LOG, "a");
        sub_404eb1_SetLogFile(global_dd_408588_logFile);
        sub_404ec5_EnableLog(LOG_TYPE_FILE);
        // add esp, 10h ��ƽ��ǰ�������C����������ɵ�ջ�仯
    }

    sub_406c59_CheckGameExeVersion();
    sub_404ed0_LogFormat(LOG_TAG(WinMain), CSTR_LOG_START_SEPARATOR_LINE);
    sub_404ed0_LogFormat(LOG_TAG(WinMain), CSTR_D2_LOADER_AUTHOR_STATEMENT);
    char* commandLine = GetCommandLineA();
    sub_404ed0_LogFormat(LOG_TAG(WinMain), "Command Line: %s", commandLine);
    sub_404ed0_LogFormat(LOG_TAG(WinMain), "Loader Version: %s", CSTR_D2_LOADER_VERSION_AND_BUILD);
    sub_404ed0_LogFormat(LOG_TAG(WinMain), "Client Version: 0x%08X", global_dd_408620_settings->dd_07b4_gameProductVersionFlag.value);
    const char* mode = global_dd_408620_settings->db_0000_expansion.value ? "Expansion" : "Classic";
    sub_404ed0_LogFormat(LOG_TAG(WinMain), "Running in %s Mode", mode);
    
    sub_404c57_GameMain();

    sub_404eb1_SetLogFile(NULL);

    if (global_dd_408588_logFile)
    {
        fclose(global_dd_408588_logFile);
    }

    return 1;
}
