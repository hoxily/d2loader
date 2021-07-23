#include "pch.h"
#include "sub_40a840.h"
#include "../global-variables.h"

/*
* 子类化 GameWindow 窗体。
* 参见 Subclassing a Window：https://docs.microsoft.com/en-us/windows/win32/winmsg/using-window-procedures#subclassing-a-window
*/
extern LRESULT APIENTRY sub_40a840_GameWindowSubClassProc(
    HWND hwnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
)
{
    // 参见 WM_ACTIVATEAPP 消息说明：https://docs.microsoft.com/en-us/windows/win32/winmsg/wm-activateapp
    if (uMsg == WM_ACTIVATEAPP && wParam == FALSE)
    {
        // 忽略切换应用程序导致当前窗体反激活的消息，屏蔽原先的窗体最小化逻辑。
        return 0;
    }
    else
    {
        // 其他消息使用原始窗体处理函数。
        return global_dd_40a7c8(
            global_dd_40a7c2_originGameWindowProc,
            hwnd,
            uMsg,
            wParam,
            lParam
        );
    }
}