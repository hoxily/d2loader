#include <Windows.h>
#include <assert.h>
#include "logger.h"

/* global variable */
struct version_info* Dst;

/* string constant */
#define D2_EXP_DOT_MPQ "d2exp.mpq"
#define D2_X_TALK_DOT_MPQ "d2xtalk.mpq"

/* function prototype */

/* struct */
#pragma pack(1) // ���ֽڶ��룬���㶨λ
struct version_info
{
    BYTE offset_0_expansion;
    BYTE _0[0x20d - 1];
    BOOL (*offset_20d_IsExpansion)();
    BYTE _[0xc94 - 0x20d - 4];
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
    return Dst->offset_0_expansion;
}

void sub_4069d8()
{
    memset(Dst, 0, sizeof(struct version_info));
    if (sub_40735e_CheckExpansion())
    {
        Dst->offset_0_expansion = TRUE;
    }

    Dst->offset_20d_IsExpansion = sub_406bab_IsExpansion;
    //TODO
}

void* sub_406803()
{
    assert(sizeof(struct version_info) == 0xc94);
    Dst = malloc(sizeof(struct version_info));
    if (!Dst)
    {
        return NULL;
    }
    sub_4069d8();
    //TODO
}

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd)
{
    InitLogFile();

    Dst = sub_406803();
    if (!Dst)
    {
        return 0;
    }
    return 0;
}