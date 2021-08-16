#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <stdarg.h>

const bool ENABLE_TAGS = false;

extern "C" {

    typedef BOOL(__stdcall* fn_RunPlugin)(DWORD reasonFlag, DWORD* gameProductVersionFlag);

    BOOL __stdcall PluginMain(DWORD reasonFlag, DWORD* gameProductVersionFlag);

#define RUN_PLUGIN_REASON_INIT 0x1ul
#define RUN_PLUGIN_REASON_CLEANUP 0x2ul
#define RUN_PLUGIN_REASON_ENTER_GAME 0x4ul
#define RUN_PLUGIN_REASON_LEAVE_GAME 0x8ul
#define RUN_PLUGIN_REASON_ENTER_CHANNEL 0x10ul
#define RUN_PLUGIN_REASON_LEAVE_CHANNEL 0x20ul
#define RUN_PLUGIN_REASON_ENTER_MAIN_MENU 0x40ul
#define RUN_PLUGIN_REASON_LEAVE_MAIN_MENU 0x80ul
#define RUN_PLUGIN_REASON_ERROR_CLEANUP 0x100ul

    struct query_interface_result
    {
        // 0x44320000
        DWORD magic;
        // 0x01000912
        DWORD version;
        const char* pluginName;
        fn_RunPlugin runPlugin;
    };

    struct query_interface_result qir = {
        0x44320000,
        0x01000912,
        "D2ItemDescriptionCopier",
        PluginMain
    };

    struct query_interface_result* __stdcall QueryInterface();

    void __fastcall MyDrawHoverText(wchar_t* wStr, long xPos, long yPos, DWORD dwTran, DWORD dwColor);
    void __fastcall D2Win_DrawHoverText(wchar_t* wStr, long xPos, long yPos, DWORD dwTran, DWORD dwColor);
}

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

void PatchD2Win_10085()
{
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

char* g_LastBuffer = nullptr;
const int MAX_LINE = 4096;
char* g_lines[MAX_LINE];
int g_lineCount = 0;

void CutToLines(char* str)
{
    g_lineCount = 0;
    char* line = str;
    for (char* p = str; *p != '\0'; p++)
    {
        char ch = *p;
        if (ch == '\n' && g_lineCount < MAX_LINE)
        {
            g_lines[g_lineCount++] = line;
            *p = '\0';
            line = p + 1;
        }
    }
    if (g_lineCount < MAX_LINE)
    {
        g_lines[g_lineCount++] = line;
    }
}

void ReverseLines()
{
    int mid = g_lineCount / 2;
    for (int left = 0, right = g_lineCount - 1; left < right; left++, right--)
    {
        char* tmp = g_lines[left];
        g_lines[left] = g_lines[right];
        g_lines[right] = tmp;
    }
}

const int BB_CODE_LINE_BUFFER_SIZE = 40960;
char g_bbCodeLineBuffer[BB_CODE_LINE_BUFFER_SIZE];
const int SNPRINTF_BUFFER_SIZE = 4096;
char g_snprintfBuffer[SNPRINTF_BUFFER_SIZE];
std::vector<std::string> g_bbCodeLines;

const char* D2ColorCodeToHexColorString(char colorCode)
{
    switch (colorCode)
    {
    case '1': // 红色
        return "#b04434";
    case '2': // 绿色
        return "#18fc00";
    case '3': // 蓝色
        return "#5050ac";
    case '4': // 暗金
        return "#9c885c";
    case '5': // 灰色
        return "#505050";
    case '9': // 金色
        return "#d8b864";
    case ':': // 暗绿
        return "#186408";
    case ';': // 紫红
        return "#a420fc";
    case '0': // 白色
    default:
        //return "#c4c4c4";
        return "#ffffff"; // 在亮底背景上，需要纯白才看得清。
    }
}

void BufferPrintf(const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    vsnprintf(g_snprintfBuffer, SNPRINTF_BUFFER_SIZE, format, ap);
    va_end(ap);
    //printf("BufferPrintf lineBuffer: %s\n", g_snprintfBuffer);
    strcat(g_bbCodeLineBuffer, g_snprintfBuffer);
    //printf("BufferPrintf bbCodeLineBuffer: %s\n", g_bbCodeLineBuffer);
}

void BeginColorTag(char colorCode)
{
    BufferPrintf("[color=%s]", D2ColorCodeToHexColorString(colorCode));
}

void EndColorTag()
{
    BufferPrintf("[/color]");
}

char FormatStringWithColorCode(char* str, char initColorCode)
{
    int len = strlen(str);
    int subStringLength = 0;
    char colorCode = initColorCode;
    for (int i = 0; i < len; i++)
    {
        if (str[i] == '?' && str[i + 1] == 'c')
        {
            char newColorCode = str[i + 2];
            if (newColorCode == colorCode)
            {
                i += 2;
                continue;
            }
            else
            {
                str[subStringLength] = '\0';
                if (ENABLE_TAGS)
                {
                    BeginColorTag(colorCode);
                }
                BufferPrintf("%s", str);
                if (ENABLE_TAGS)
                {
                    EndColorTag();
                }
                colorCode = newColorCode;
                i += 2;
                subStringLength = 0;
                continue;
            }
        }
        str[subStringLength++] = str[i];
    }
    str[subStringLength] = '\0';
    if (subStringLength > 0)
    {
        if (ENABLE_TAGS)
        {
            BeginColorTag(colorCode);
        }
        BufferPrintf("%s", str);
        if (ENABLE_TAGS)
        {
            EndColorTag();
        }
    }
    return colorCode;
}

void BeginAlignTag(const char* align)
{
    BufferPrintf("[align=%s]", align);
}

void EndAlignTag()
{
    BufferPrintf("[/align]");
}

void BeginBoldTag()
{
    BufferPrintf("[b]");
}

void EndBoldTag()
{
    BufferPrintf("[/b]");
}

void BeginSizeTag(const char* size)
{
    BufferPrintf("[size=%s]", size);
}

void EndSizeTag()
{
    BufferPrintf("[/size]");
}

void BeginLine()
{
    g_bbCodeLineBuffer[0] = '\0';
}

void EndLine()
{
    g_bbCodeLines.push_back(g_bbCodeLineBuffer);
}

void FormatString(const char* str, BOOL hasColor)
{
    // str is line by line inverted, reverse it;
    char* dupStr = _strdup(str);
    CutToLines(dupStr);
    // 不能直接翻转，因为颜色代码是顺序。实际显示画面是逆序。
    //ReverseLines();
    g_bbCodeLines.clear();
    char colorCode = '0';
    for (int i = 0; i < g_lineCount; i++)
    {
        BeginLine();
        if (ENABLE_TAGS)
        {
            BeginAlignTag("center");
            BeginBoldTag();
            BeginSizeTag("5");
        }
        colorCode = FormatStringWithColorCode(g_lines[i], colorCode);
        if (ENABLE_TAGS)
        {
            EndSizeTag();
            EndBoldTag();
            EndAlignTag();
        }
        EndLine();
    }

    //printf("bbCodeLines.size() = %d\n", g_bbCodeLines.size());
    std::string totalStr = "";
    for (int i = g_bbCodeLines.size() - 1; i >= 0; i--)
    {
        if (ENABLE_TAGS)
        {
            totalStr = totalStr + g_bbCodeLines[i];
        }
        else
        {
            totalStr = totalStr + "\n" + g_bbCodeLines[i];
        }
    }
    strcpy(g_bbCodeLineBuffer, totalStr.c_str());
    printf("%s\n", g_bbCodeLineBuffer);
    /*if (OpenClipboard(NULL))
    {
        SetClipboardData(CF_TEXT, g_bbCodeLineBuffer);
        CloseClipboard();
    }*/
    free(dupStr);
}

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
    int bufferSize = WideCharToMultiByte(
        CP_ACP,
        0,
        wStr,
        -1, // auto calculate wStr length
        nullptr,
        0, // to return the required bufferSize
        nullptr,
        nullptr
    );
    if (bufferSize == 0)
    {
        printf("WideCharToMultiByte failed with error: 0x%08x\n", GetLastError());
        return;
    }
    char* buffer = (char*)malloc(bufferSize);
    memset(buffer, 0, bufferSize);

    WideCharToMultiByte(
        CP_ACP,
        0,
        wStr,
        -1, // auto calculate wStr length
        buffer,
        bufferSize,
        nullptr,
        nullptr
    );


    if (g_LastBuffer != nullptr && strcmp(buffer, g_LastBuffer) == 0)
    {
        //printf("duplicated call. Skip it.\n");
        return;
    }
    if (g_LastBuffer != nullptr)
    {
        free(g_LastBuffer);
    }

    g_LastBuffer = buffer;

    printf("calling MyDrawHoverText with following arguments:\n");
    printf("wStr: %s\n", buffer);
    printf("xPos: %d\n", xPos);
    printf("yPos: %d\n", yPos);
    printf("dwTran: %u\n", dwTran);
    printf("dwColor: %u\n", dwColor);

    FormatString(buffer, dwColor);
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
        InitConsoleOutput();
        PatchD2Win_10085();
    }


    printf("run PluginMain with reason 0x%08x\n", reasonFlag);
    return TRUE;
}