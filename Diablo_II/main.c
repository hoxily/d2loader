#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <assert.h>
#include "resource.h"
#include "functions/sub_40146d.h"

#define DIABLO_II_EVENT_NAME "DIABLO_II_OK"

DWORD global_dd_406034 = 0x22;

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    PSTR lpCmdLine,
    INT nCmdShow)
{
    HANDLE hObject = CreateEventA(NULL, FALSE, FALSE, DIABLO_II_EVENT_NAME);
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        MessageBoxA(NULL, "Event(DIABLO_II_OK) is already exist.", "tips", MB_OK);
        CloseHandle(hObject);
        return 0;
    }

    STARTUPINFOA startupInfo;
    /*
    * f3 ab: rep stosd
    * fill ecx doublewords at es:edi with eax
    */
    assert(sizeof(startupInfo) / sizeof(DWORD) == 0x11);
    memset(&startupInfo, 0, sizeof(startupInfo));
    assert(MAX_PATH == 0x104);
    char bufferGameExe[MAX_PATH];
    PROCESS_INFORMATION processInformation;
    memset(&processInformation, 0, sizeof(processInformation));

    LoadStringA(hInstance, IDS_1_GAME_DOT_EXE, bufferGameExe, ARRAYSIZE(bufferGameExe));
    char bufferDiabloIiExe[MAX_PATH];
    LoadStringA(hInstance, IDS_7_DIABLO_II_DOT_EXE, bufferDiabloIiExe, ARRAYSIZE(bufferDiabloIiExe));
    char* myCommandLine = GetCommandLineA();
    MessageBoxA(NULL, myCommandLine, "CommandLine", MB_OK);
    char* pDiabloIiExe = strstr(myCommandLine, bufferDiabloIiExe);
    if (!pDiabloIiExe)
    {
        MessageBoxA(NULL, "This Exe should be named 'Diablo II.exe'", "Error", MB_OK);
        return 0;
    }
    char bufferNewCommandLine[MAX_PATH] = { 0 };
    strncpy(bufferNewCommandLine, myCommandLine, pDiabloIiExe - myCommandLine);
    strcat(bufferNewCommandLine, bufferGameExe);
    strcat(bufferNewCommandLine, pDiabloIiExe + strlen(bufferDiabloIiExe));
    return 0;
}