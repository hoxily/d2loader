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
        * ��ǰ�������� MessageBoxA ʱѹջ�Ĳ����Ƚ���֡���ѹ��ջ�У�Ȼ���¼���Ĵ������ٴ�ѹջ��
        * ��IDA����ظ��ú�������������ֲ�������lpText��lpCaption��
        * ���Ե����Ҽ��˵��еġ�Edit function...�����ڵ�����Edit function�Ի����н� 
        * Local variables area ����Ϊ��ȷ�Ĵ�С��
        * 
        * push offset ��������
        * push offset ��������
        * push 10h ; �� MB_ICONERROR
        * mov eax, [esp+8] ; eax ָ��ո�push��������������
        * mov ebx, [esp+4] ; ebx ָ��ո�push�����ı�������
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
    // ������õ���ຯ���������ڡ���Ҫ�� sub_4070d5_D2CommonInit �л�ȡ��
    // ������ʹ��ecx��edx�Ĵ��������ݺ���������
    // ����������� __fastcall ����Լ�����μ� https://docs.microsoft.com/en-us/cpp/cpp/fastcall?view=msvc-160
    // 1. ǰ����С�ڵ���DWORD��С�Ĳ���������˳�򣬷ֱ�����ecx��edx��
    // 2. ����ʣ��������ҵ����˳��push��ջ�ϡ�
    // 3. �����ú�����������ջ�ϵĲ�����
    
    // ����ĺ�������Լ���Լ������������ܹ⿴ d2loader �Ļ����롣
    // ��Ҫ��϶�Ӧ��dll�Ĵ������ȷ�ϡ�

    var_8 = global_dd_408688("D2");

    // ����ע�����
    // ��Ҫ�������Ե�d2loader.exe����ΪWindows XP SP3�����ԡ�
    // Ȼ��Visual Studio��Ҫ�Թ���ԱȨ����������������������������Stack overflow�쳣��
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
        // �˴���Ȼ������ settings ���󣬵��Ǹú����ڲ�ֻ�Ƕ�ȡ�������� 0x20d ƫ�ƴ��ĺ���ָ�롣
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

    // ע�⣺
    // ��������п��ܻ����ʧ�ܡ����� ACCESS VIOLATION (0xC0000005) at һ������ĵ�ַ����
    // ���ʱ����Ҫ����һ��windowsϵͳ��
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

    // ����: �� gamma �ֶε�ƫ�������˴��Ļ�����������в����� global_dd_402ea8_CommandLineArgumentTable �г�ͻ��
    // �����Դ���Ϊ׼��
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
        // ������� global_dd_4086cc ���ô���edx ����Ϊ 0��
        // ���ҵ�ĿǰΪֹ��û�иĹ� edx������ edx ��ǰӦ��Ϊ 0
        var_4 = global_dd_40868c(
            global_dd_408620_settings->db_0000_expansion.value,
            0
        );

        // ���ʣ�Sound System ����ֵΪ 0. ����ʵ�������������ġ�
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
