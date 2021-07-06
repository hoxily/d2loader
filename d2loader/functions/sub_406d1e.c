#include "sub_406d1e.h"
#include "../global-variables.h"
#include "../storm-api.h"
#include "sub_4070d5.h"
#include "sub_404ed0.h"


BOOL sub_406d1e_D2Init(
)
{
    if (!sub_4070d5_D2CommonInit())
    {
        /*
        * 当前函数调用 MessageBoxA 时压栈的操作比较奇怪。先压入栈中，然后记录到寄存器，再次压栈。
        * 让IDA错误地给该函数添加了两个局部变量：lpText和lpCaption。
        * 可以单击右键菜单中的“Edit function...”，在弹出的Edit function对话框中将 
        * Local variables area 调整为正确的大小。
        * 
        * push offset 内容文字
        * push offset 标题文字
        * push 10h ; 即 MB_ICONERROR
        * mov eax, [esp+8] ; eax 指向刚刚push进来的内容文字
        * mov ebx, [esp+4] ; ebx 指向刚刚push进来的标题文字
        * push ebx
        * push eax
        * push 0
        * call ds:MessageBoxA
        */
         
        MessageBoxA(
            NULL,
            "Common Init Failed",
            LOG_TAG,
            MB_ICONERROR
        );
        return FALSE;
    }

    if (global_dd_408620_settings->db_0988_mpqFile.value[0] != '\0')
    {
        BOOL success = dll_storm_266(
            global_dd_408620_settings->db_0988_mpqFile.value,
            0x10000,
            2,
            &global_dd_408638_mpqFileHandle
        );
        if (success)
        {
            sub_404ed0_LogFormat(
                LOG_TAG,
                "Added MPQ '%s'",
                global_dd_408620_settings->db_0988_mpqFile.value
            );
        }
        else
        {
            global_dd_408638_mpqFileHandle = 0;
            sub_404ed0_LogFormat(
                LOG_TAG,
                "Failed to open MPQ '%s'",
                global_dd_408620_settings->db_0988_mpqFile.value
            );
        }
    }

    DWORD titleEnabled;
    DWORD videoMode;
    if (global_dd_408620_settings->db_0004_window.value)
    {
        videoMode = 1;
        const char* withOrWithout = NULL;
        if (global_dd_408620_settings->db_079f_noTitle.value)
        {
            titleEnabled = FALSE;
            withOrWithout = "Without";
        }
        else
        {
            titleEnabled = TRUE;
            withOrWithout = "With";
        }
        sub_404ed0_LogFormat(
            LOG_TAG,
            "Video Mode: Window Mode (%s Title)",
            withOrWithout
        );
    }
    else
    {
        titleEnabled = FALSE;
        if (global_dd_408620_settings->db_0008_rave.value)
        {
            videoMode = 6;
        }
        else
        {
            if (global_dd_408620_settings->db_0005.value)
            {
                videoMode = 4;
            }
            else
            {
                videoMode = 3;
            }
        }

        sub_404ed0_LogFormat(
            LOG_TAG,
            "Video Mode: Full Screen Mode"
        );
    }

    DWORD edi_isRes800 = FALSE;
    if (global_dd_408620_settings->db_07a0_res800.value)
    {
        edi_isRes800 = TRUE;
    }
    // 下面调用的许多函数都不存在。需要在 sub_4070d5_D2CommonInit 中获取。
    // 看起来使用ecx、edx寄存器来传递函数参数。
    //TODO
    return FALSE;
}
