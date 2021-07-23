#include "pch.h"
#include "sub_40a840.h"
#include "../global-variables.h"

/*
* ���໯ GameWindow ���塣
* �μ� Subclassing a Window��https://docs.microsoft.com/en-us/windows/win32/winmsg/using-window-procedures#subclassing-a-window
*/
extern LRESULT APIENTRY sub_40a840_GameWindowSubClassProc(
    HWND hwnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
)
{
    // �μ� WM_ACTIVATEAPP ��Ϣ˵����https://docs.microsoft.com/en-us/windows/win32/winmsg/wm-activateapp
    if (uMsg == WM_ACTIVATEAPP && wParam == FALSE)
    {
        // �����л�Ӧ�ó����µ�ǰ���巴�������Ϣ������ԭ�ȵĴ�����С���߼���
        return 0;
    }
    else
    {
        // ������Ϣʹ��ԭʼ���崦������
        return global_dd_40a7c8(
            global_dd_40a7c2_originGameWindowProc,
            hwnd,
            uMsg,
            wParam,
            lParam
        );
    }
}