#include "pch.h"
#include "sub_40a7ce.h"
#include <assert.h>
#include "../global-variables.h"
#include "sub_40a840.h"
#include "sub_404ed0.h"

typedef LONG (WINAPI *fn_SetWindowLongA)(
    _In_ HWND hWnd,
    _In_ int nIndex,
    _In_ LONG dwNewLong);

void sub_40a7ce_SetGameWindowNoHide(
)
{
    sub_404ed0_LogFormat(
        LOG_TAG,
        "patch game window proc to no hide on deactivate"
    );

    HMODULE moduleUser32 = GetModuleHandleA("User32.dll");
    assert(moduleUser32 != NULL);
    global_dd_40a7c8 = (fn_CallWindowProcA)GetProcAddress(moduleUser32, "CallWindowProcA");
    fn_SetWindowLongA fn = (fn_SetWindowLongA)GetProcAddress(moduleUser32, "SetWindowLongA");
    global_dd_40a7c2_originGameWindowProc = (WNDPROC)fn(
        global_dd_408634_gameWindowHandle, // hWnd
        GWL_WNDPROC, // nIndex
        (LPARAM)sub_40a840_GameWindowSubClassProc // dwNewLong
    );
}