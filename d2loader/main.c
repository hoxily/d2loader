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
    } db_07af;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x7b4];
        DWORD value;
    } dw_07b4_gameProductVersionFlag;

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
    global_dd_408620_settings->db_07af.value = TRUE;
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
        &data,
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
    global_dd_408620_settings->dw_07b4_gameProductVersionFlag.value = esi;
}

BOOL sub_404c57()
{
    //TODO
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
    sub_404ed0_LogFormat(LOG_TAG(WinMain), "Client Version: 0x%08X", global_dd_408620_settings->dw_07b4_gameProductVersionFlag.value);
    const char* mode = global_dd_408620_settings->db_0000_expansion.value ? "Expansion" : "Classic";
    sub_404ed0_LogFormat(LOG_TAG(WinMain), "Running in %s Mode", mode);
    
    sub_404c57();

    sub_404eb1_SetLogFile(NULL);

    if (global_dd_408588_logFile)
    {
        fclose(global_dd_408588_logFile);
    }

    return 1;
}