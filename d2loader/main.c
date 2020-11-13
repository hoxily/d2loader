#include <Windows.h>
#include <assert.h>
#include "logger.h"
#include "constant-strings.h"

/* global variable */
union version_info* Dst;

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
    } expansion;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x20d];
        BOOL (*value)();
    } IsExpansion;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x79d];
        BYTE value;
    } flag_79d;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x7af];
        BYTE value;
    } flag_7af;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x7bc];
        char value[7 + 1];
    } bnormal;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x7ec];
        char value[9 + 1];
    } gameName;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x804];
        char value[38 + 1];
    } title;

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
    return Dst->expansion.value;
}

void sub_4069d8()
{
    memset(Dst, 0, sizeof(union version_info));
    if (sub_40735e_CheckExpansion())
    {
        Dst->expansion.value = TRUE;
    }

    Dst->IsExpansion.value = sub_406bab_IsExpansion;
    Dst->flag_79d.value = TRUE;
    Dst->flag_7af.value = TRUE;
    strcpy(Dst->title.value, D2_LOADER_VERSION_AND_BUILD);
    strcpy(Dst->gameName.value, DIABLO_II);
    strcpy(Dst->bnormal.value, B_NORMAL);
}

void* sub_406803()
{
    assert(sizeof(union version_info) == 0xc94);
    Dst = malloc(sizeof(union version_info));
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