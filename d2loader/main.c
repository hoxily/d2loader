#include <Windows.h>
#include "logger.h"

/* global variable */
void* Dst;

/* string constant */
#define D2_EXP_DOT_MPQ "d2exp.mpq"
#define D2_X_TALK_DOT_MPQ "d2xtalk.mpq"

/* function prototype */


/**
 * sub_407380
 */
BOOL CheckFileExist(const char* filePath)
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

void sub_40735e()
{
    if (!CheckFileExist(D2_EXP_DOT_MPQ))
    {
        LogFormat("%s not exist.\n", D2_EXP_DOT_MPQ);
        return;
    }
    if (!CheckFileExist(D2_X_TALK_DOT_MPQ))
    {
        LogFormat("%s not exist.\n", D2_X_TALK_DOT_MPQ);
        return;
    }
}

void sub_4069d8()
{
    memset(Dst, 0, 0xc94);
    sub_40735e();
    //TODO
}

void* sub_406803()
{
    Dst = malloc(0xc94);
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