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

    DWORD edi_isRes800 = FALSE;
    if (global_dd_408620_settings->db_07a0_res800.value)
    {
        edi_isRes800 = TRUE;
    }
    // ������õ���ຯ���������ڡ���Ҫ�� sub_4070d5_D2CommonInit �л�ȡ��
    // ������ʹ��ecx��edx�Ĵ��������ݺ���������
    //TODO
    return FALSE;
}
