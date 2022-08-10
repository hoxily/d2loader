#include <Windows.h>
#include "resource.h"

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    PSTR lpCmdLine,
    INT nCmdShow)
{
    MessageBoxA(NULL, "this is diablo2 clone project.", "d2-clone", MB_OK);

    HANDLE hObject = CreateEventA(NULL, FALSE, FALSE, "DIABLO_II_OK");
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        MessageBoxA(NULL, "Event(DIABLO_II_OK) is already exist.", "tips", MB_OK);
        CloseHandle(hObject);
        return 0;
    }
    wchar_t buffer[256];
    for (UINT id = 1; id <= 9; id++)
    {
        int length = LoadStringW(hInstance, id, buffer, sizeof(buffer) / sizeof(buffer[0]));
        MessageBoxW(NULL, buffer, L"i", MB_OK);
    }
    return 0;
}