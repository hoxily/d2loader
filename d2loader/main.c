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
}

void sub_404eb1_setLogFile(FILE* fp)
{
    global_dw_40858c_logFile = fp;
}

void sub_404ec5_setValue(DWORD num)
{
    // ���� global_dw_408590 �ĳ�ʼֵΪ 0�����Դ˴��İ�λ��ȼ��ڽ�����ֵΪ����� 0x20
    global_dw_408590 |= num;
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
        logFile = fopen("d2loader.log", "a");
        sub_404eb1_setLogFile(logFile);
        sub_404ec5_setValue(0x20);
        // add esp, 10h ��ƽ��ǰ�������C����������ɵ�ջ�仯
    }
    //TODO
    return 0;
}