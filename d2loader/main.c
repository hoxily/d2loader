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
union version_info* global_dd_408620_Dst;
FILE* global_dd_408588_logFile;
FILE* global_dd_40858c_logFile;

#define LOG_TYPE_FILE 0x20
#define LOG_TYPE_CONSOLE 0x10

DWORD global_dd_408590_logFlag;

//TODO ��Ҫ������indexTable
DWORD global_dd_402eb8_indexTable[461];

void* global_dd_402eac = 0x004035f0;

void* global_dd_402eb0 = 0x004035f8;

/* function prototype */

/* struct */

// ������ʹ��union��ֱ������÷Ѿ���˼���ֶε�ƫ�����ˡ�
#pragma pack(1) // ���ֽڶ��룬���㶨λ
union version_info
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
    // �ֲ�����ǰ��windows������������鿴d2loader.exe�����ȼ���������ʾΪ����������
    strcpy(global_dd_408620_Dst->db_07bc_processPriority.value, CSTR_PROCESS_PRIORITY_BELOW_NORMAL);
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

int sub_406ac0(const char* buffer)
{
    if (buffer == 0) {
        return -1;
    }

    if (buffer[0] != '-')
    {
        return -1;
    }

    int ebp_index = 0;
    if (global_dd_402eac == ebp_index)
    {
        return -1;
    }

    // ָ��ȥ����-���Ĳ�����
    const char* ebx_argName = buffer + 1;
    // ������ַ������ұ���Ҫ���»��㡣
    char** esi_stringTableEntry = &global_dd_402eb0;
    do
    {
        const char* entry_minus8 = esi_stringTableEntry[-2];
        if (lstrcmpiA(entry_minus8, ebx_argName) == 0)
        {
            break;
        }
        const char* entry = esi_stringTableEntry[0];
        if (lstrcmpiA(entry, ebx_argName) == 0)
        {
            break;
        }
        esi_stringTableEntry += 5;
        ebp_index++;
    } while (esi_stringTableEntry[-1] != NULL);

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
        int i = sub_406ac0(buffer);
        // test esi, esi
        // jl
        // ��������ϱ�ʾ���esi�Ƿ�Ϊ������
        if (i >= 0)
        {
            if (global_dd_402eb8_indexTable[i * 5] != 1)
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
        // add esp, 10h ��ƽ��ǰ�������C����������ɵ�ջ�仯
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