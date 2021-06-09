#include <Windows.h>
#include <assert.h>
#include <stdio.h>
#include "logger.h"
#include "constant-strings.h"

/* global variable */
union version_info* Dst;
FILE* logFile;
FILE* global_dw_40858c_logFile;
DWORD global_dw_408590;

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
        BYTE offset[0x7bc];
        char value[7 + 1];
    } db_07bc_bnormal;

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
    if (!sub_407380_CheckFileExist(D2_EXP_DOT_MPQ))
    {
        LogFormat("%s not exist.\n", D2_EXP_DOT_MPQ);
        return FALSE;
    }
    if (!sub_407380_CheckFileExist(D2_X_TALK_DOT_MPQ))
    {
        LogFormat("%s not exist.\n", D2_X_TALK_DOT_MPQ);
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
    if (Dst == NULL)
    {
        return FALSE;
    }
    return Dst->db_0000_expansion.value;
}

void sub_4069d8()
{
    memset(Dst, 0, sizeof(union version_info));
    if (sub_40735e_CheckExpansion())
    {
        Dst->db_0000_expansion.value = TRUE;
    }

    Dst->dd_020d_IsExpansion.value = sub_406bab_IsExpansion;
    Dst->db_079d.value = TRUE;
    Dst->db_07af.value = TRUE;
    strcpy(Dst->db_0804_title.value, D2_LOADER_VERSION_AND_BUILD);
    strcpy(Dst->db_07ec_gameName.value, DIABLO_II);
    strcpy(Dst->db_07bc_bnormal.value, B_NORMAL);
}

char* sub_406a68(char* arg0, char* buffer)
{
    if (arg0 == NULL)
    {
        return NULL;
    }
    char* p = arg0;
    char* pBuffer = buffer;
    BOOL esi = FALSE;
    if (*p == '\0')
    {
        buffer[0] = '\0';
        return p;
    }
    else
    {
        while (*p != '\0')
        {
            if (*p == '\"')
            {
                esi = TRUE;
            }
            else
            {
                if (*p == ' ')
                {

                }
            }
            p++;
        }
    }
}

BOOL sub_406887(char* commandLine)
{
    char buffer[0x100];
    sub_406a68(commandLine, buffer);
    //TODO
}

BOOL sub_4068f2(char* filename)
{
    //TODO
}

BOOL sub_406803()
{
    assert(sizeof(union version_info) == 0xc94);
    Dst = malloc(sizeof(union version_info));
    if (!Dst)
    {
        return FALSE;
    }
    sub_4069d8();
    char* commandLine = GetCommandLineA();
    if (!sub_406887(commandLine))
    {
        return FALSE;
    }
    sub_4068f2(&Dst->db_0884_filename.value);
    //TODO
    return TRUE;
}

void sub_404eb1_setLogFile(FILE* fp)
{
    global_dw_40858c_logFile = fp;
}

void sub_404ec5_setValue(DWORD num)
{
    // 由于 global_dw_408590 的初始值为 0，所以此处的按位或等价于将它赋值为传入的 0x20
    global_dw_408590 |= num;
}

void sub_406c59()
{
    // GetFileVersionInfoSizeA 总是会把 unusedHandle 设为 0
    DWORD unusedHandle;
    UINT len;
    VS_FIXEDFILEINFO* fileInfo;

    DWORD edi_size = GetFileVersionInfoSizeA(GAME_DOT_EXE, &unusedHandle);
    void* ebx_ptr = VirtualAlloc(NULL, edi_size, MEM_COMMIT, PAGE_READWRITE);
    if (ebx_ptr) {
        // esi 已经在上面被 xor esi, esi 置为0；
        if (GetFileVersionInfoA(GAME_DOT_EXE, 0, edi_size, ebx_ptr))
        {
            if (VerQueryValueA(ebx_ptr, "\\", &fileInfo, &len))
            {
                if (fileInfo)
                {
                    // dwFileVersionLS 的偏移量为 0x0c
                    // 检查 game.exe 的FileVersion是否为 1.0.13.60，
                    // 由于 dwFileVersionMS 必定为 0x00010000，所以只检查了 dwFileVersionLS
                    if (fileInfo->dwFileVersionLS != 0x000d003c)
                    {
                        MessageBoxA(NULL, D2_LOADER_DOES_NOT_RECOGNIZE_GAME_VERSION, UNKNOW_VERSION, MB_OK | MB_ICONERROR);
                        ExitProcess(1);
                    }
                    //TODO
                }
            }
        }
    }
    // VirtualFree 要求第一个参数不为 NULL，所以用if包裹一下。
    if (ebx_ptr)
    {
        VirtualFree(ebx_ptr, 0, MEM_RELEASE);
    }
    //TODO
}

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd)
{
    InitLogFile();

    if (!sub_406803())
    {
        return 0;
    }
    if (Dst->db_07ac_enableLogFile.value)
    {
        logFile = fopen(D2_LOADER_DOT_LOG, "a");
        sub_404eb1_setLogFile(logFile);
        sub_404ec5_setValue(0x20);
        // add esp, 10h 是平衡前面的三个C函数调用造成的栈变化
    }

    sub_406c59();
    //TODO
    return 0;
}