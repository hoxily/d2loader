#include "pch.h"
#include "main.h"
#include <stdio.h>

const bool ENABLE_TAGS = false;

struct query_interface_result qir = {
    0x44320000,
    0x01000912,
    "D2ItemDescriptionCopier",
    PluginMain
};

struct query_interface_result* __stdcall QueryInterface()
{
    return &qir;
}

void InitConsoleOutput(
)
{
    // Win32窗口程序使用控制台，参见：https://blog.csdn.net/qq_31042143/article/details/107307860
    AllocConsole();
    (void)freopen("conout$", "w", stdout);
    (void)freopen("conout$", "w", stderr);
}

static bool g_havePatchD2Win = false;

void PatchD2Win_10085()
{
    if (g_havePatchD2Win)
    {
        return;
    }
    g_havePatchD2Win = true;

    HMODULE moduleD2Win = GetModuleHandleA("d2win");
    printf("module D2Win: 0x%p\n", moduleD2Win);
    if (moduleD2Win != (void*)0x6f8e0000)
    {
        printf("Unexpected module D2Win base address. Expected base address is 0x6f8e0000\n");
        return;
    }

    DWORD oldProtect, unused;
    const unsigned int startAddress = 0x6F8F18F0;
    const SIZE_T patchSize = 5;
    VirtualProtect(
        (void*)startAddress,
        patchSize, // patch to jmp rel32 instruction: e9 xx xx xx xx
        PAGE_EXECUTE_READWRITE,
        &oldProtect
    );
    *(BYTE*)startAddress = 0xe9;
    *(int*)(startAddress + 1) = (int)&MyDrawHoverText - startAddress - patchSize;
    VirtualProtect(
        (void*)startAddress,
        patchSize,
        oldProtect,
        &unused
    );
}

void LogToFile(const wchar_t* str)
{
    int pid = GetCurrentProcessId();
    char buffer[64];
    sprintf(buffer, "draw_hover_text_%d.txt", pid);
    FILE* f = fopen(buffer, "wb");
    if (f)
    {
        fwrite(str, sizeof(wchar_t), lstrlenW(str), f);
        fclose(f);
    }
}

const int MAX_LAST_DRAW_TEXT = 4096;
static wchar_t g_lastDrawText[MAX_LAST_DRAW_TEXT] = { 0 };

void __fastcall MyDrawHoverText(wchar_t* wStr, long xPos, long yPos, DWORD dwTran, DWORD dwColor)
{
    // originally draw in game window
    D2Win_DrawHoverText(wStr, xPos, yPos, dwTran, dwColor);

    // my staff begin here
    if (wStr == nullptr)
    {
        //printf("wStr is null.\n");
        return;
    }
    if (lstrcmpW(wStr, g_lastDrawText) == 0)
    {
        return;
    }
    lstrcpynW(g_lastDrawText, wStr, MAX_LAST_DRAW_TEXT);
    g_lastDrawText[MAX_LAST_DRAW_TEXT - 1] = 0;

    LogToFile(wStr);
}

void __fastcall D2Win_DrawHoverText(wchar_t* wStr, long xPos, long yPos, DWORD dwTran, DWORD dwColor)
{
    wchar_t* const textBuffer = (wchar_t*)0x6f9a9e58;
    const int textBufferLength = 0x400;
    if (wStr == nullptr || lstrlenW(wStr) >= textBufferLength)
    {
        memset(textBuffer, 0, sizeof(wchar_t) * textBufferLength);
    }
    else
    {
        lstrcpyW(textBuffer, wStr);
    }
    *(long*)0x6F9AA658 = xPos;
    *(long*)0x6F9AA65C = yPos;
    *(DWORD*)0x6F9AA660 = dwTran;
    *(DWORD*)0x6F9AA664 = dwColor;
    *(DWORD*)0x6F9AA668 = 0;
}

BOOL __stdcall PluginMain(DWORD reasonFlag, DWORD* gameProductVersionFlag)
{
    if (reasonFlag & RUN_PLUGIN_REASON_INIT)
    {
        //InitConsoleOutput();
        PatchD2Win_10085();
    }


    //printf("run PluginMain with reason 0x%08x\n", reasonFlag);
    return TRUE;
}