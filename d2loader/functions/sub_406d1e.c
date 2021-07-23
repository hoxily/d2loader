#include "pch.h"
#include "sub_406d1e.h"
#include "../global-variables.h"
#include "../constant-strings.h"
#include "../storm-api.h"
#include "sub_4070d5.h"
#include "sub_404ed0.h"
#include "sub_4072b7.h"
#include "sub_4070aa.h"

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

    // loc_406e1c:
    int var_8;
    int var_4;
    DWORD edi_isRes800 = FALSE;
    if (global_dd_408620_settings->db_07a0_res800.value)
    {
        edi_isRes800 = TRUE;
    }
    // 下面调用的许多函数都不存在。需要在 sub_4070d5_D2CommonInit 中获取。
    // 看起来使用ecx、edx寄存器来传递函数参数。
    // 这个特征符合 __fastcall 调用约定。参见 https://docs.microsoft.com/en-us/cpp/cpp/fastcall?view=msvc-160
    // 1. 前两个小于等于DWORD大小的参数从左到右顺序，分别分配给ecx和edx；
    // 2. 其他剩余参数从右到左的顺序push到栈上。
    // 3. 被调用函数负责清理栈上的参数。
    
    // 下面的函数调用约定以及传参数量不能光看 d2loader 的汇编代码。
    // 需要结合对应的dll的代码才能确认。

    var_8 = global_dd_408688("D2");

    // 调试注意事项：
    // 需要将被调试的d2loader.exe设置为Windows XP SP3兼容性。
    // 然后Visual Studio需要以管理员权限启动。否则下面这个函数会产生Stack overflow异常。
    var_4 = global_dd_4086c8(
        global_dd_408620_settings->db_07ec_gameName.value,
        sub_4072b7_ErrorHandle,
        CSTR_D2_LOADER_VERSION_AND_BUILD,
        1
    );

    var_8 = global_dd_4086ec(
        global_dd_408620_settings->db_01ff_direct.value,
        0
    );

    var_8 = global_dd_4086d0(
        1
    );

    var_8 = global_dd_4086c4();

    if (!var_4)
    {
        MessageBoxA(
            NULL,
            "Game Server Initialize Failed",
            LOG_TAG,
            MB_ICONERROR
        );
        return FALSE;
    }

    sub_404ed0_LogFormat(
        LOG_TAG,
        "Game Server Initialized"
    );

    var_4 = global_dd_40867c();

    if (!global_dd_408620_settings->db_07a6_noChar.value)
    {
        // 此处虽然传入了 settings 对象，但是该函数内部只是读取并调用了 0x20d 偏移处的函数指针。
        var_8 = global_dd_4086cc(
            0,
            0,
            0,
            global_dd_408620_settings
        );

        var_4 = var_4 & var_8;
    }

    if (!var_4)
    {
        MessageBoxA(
            NULL,
            "Couldn't load Diablo 2 MPQ files. Please insert your Diablo II CD.",
            LOG_TAG,
            MB_ICONERROR
        );
        return FALSE;
    }

    sub_404ed0_LogFormat(
        LOG_TAG,
        "Game Data Files Initialized"
    );

    // 注意：
    // 这个函数有可能会调用失败。会以 ACCESS VIOLATION (0xC0000005) at 一个随机的地址报错。
    // 这个时候需要重启一下windows系统。
    var_4 = global_dd_40869c(
        GetModuleHandleA(NULL),
        videoMode,
        1,
        1
    );

    if (!var_4)
    {
        MessageBoxA(
            NULL,
            "Graphic System Initialize Failed",
            LOG_TAG,
            MB_ICONERROR
        );
        return FALSE;
    }

    sub_404ed0_LogFormat(
        LOG_TAG,
        "Graphic System Initialized"
    );

    if (global_dd_408620_settings->db_0009_perspective.value && videoMode >= 4)
    {
        global_dd_4086c0(
            1
        );
        sub_404ed0_LogFormat(
            LOG_TAG,
            "Hardware Accel Perspective Mode Enabled"
        );
    }

    var_4 = global_dd_4086d4(
        titleEnabled,
        edi_isRes800
    );

    if (!var_4)
    {
        MessageBoxA(
            NULL,
            "Failed to Create Game Client Window",
            LOG_TAG,
            MB_ICONERROR
        );
        return FALSE;
    }

    sub_404ed0_LogFormat(
        LOG_TAG,
        "Game Client Window Created Successfully"
    );

    if (global_dd_408620_settings->db_000a_lowQuality.value)
    {
        global_dd_408674();
        sub_404ed0_LogFormat(
            LOG_TAG,
            "Low Quality Video Mode Enabled"
        );
    }

    // 警告: 对 gamma 字段的偏移量，此处的汇编代码和命令行参数表 global_dd_402ea8_CommandLineArgumentTable 有冲突。
    // 这里以代码为准。
    if (global_dd_408620_settings->dd_000b_gamma.value)
    {
        global_dd_4086b0(
            global_dd_408620_settings->dd_000b_gamma.value
        );
        sub_404ed0_LogFormat(
            LOG_TAG,
            "Setting Video Gamma to %d",
            global_dd_408620_settings->dd_000b_gamma.value
        );
    }

    if (global_dd_408620_settings->db_000f_vSync.value)
    {
        global_dd_4086e4();
        sub_404ed0_LogFormat(
            LOG_TAG,
            "VSync Video Mode Enabled"
        );
    }

    if (global_dd_408620_settings->db_021b_noSound.value)
    {
        sub_404ed0_LogFormat(
            LOG_TAG,
            "Sound Have Been Disabled"
        );
    }
    else
    {
        // 在上面的 global_dd_4086cc 调用处，edx 被置为 0；
        // 并且到目前为止都没有改过 edx，所以 edx 当前应该为 0
        var_4 = global_dd_40868c(
            global_dd_408620_settings->db_0000_expansion.value,
            0
        );

        // 疑问：Sound System 返回值为 0. 但是实际上是有声音的。
        if (var_4)
        {
            sub_404ed0_LogFormat(
                LOG_TAG,
                "Sound System Initialized"
            );
        }
        else
        {
            sub_404ed0_LogFormat(
                LOG_TAG,
                "Failed to Initialize Sound System"
            );
        }
    }

    global_dd_408634_gameWindowHandle = global_dd_4086ac();

    sub_404ed0_LogFormat(
        LOG_TAG,
        "Set Window Title to \"%s\"",
        global_dd_408620_settings->db_0804_title.value
    );

    if (global_dd_408634_gameWindowHandle)
    {
        SetWindowTextA(
            global_dd_408634_gameWindowHandle,
            global_dd_408620_settings->db_0804_title.value
        );
    }

    global_dd_408630_moduleD2Multi = sub_4070aa_D2LoadLibrary("d2multi.dll");

    return global_dd_408630_moduleD2Multi != NULL;
}
