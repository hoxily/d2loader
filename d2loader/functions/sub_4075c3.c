#include "pch.h"
#include "sub_4075c3.h"
#include "../global-variables.h"
#include "sub_404ed0.h"

BOOL sub_4075c3_D2LocaleSetLocale(
    const char* locale
)
{
    if (locale == NULL)
    {
        return FALSE;
    }

    if (locale[0] == '\0' || global_dd_404718 == 0xffffffff)
    {
        sub_404ed0_LogFormat(
            LOG_TAG,
            "Using Default Locale"
        );
        return TRUE;
    }

    //TODO: ��û�������￴���κ���Ч�ĺ������á���ֻ�������˼���ȫ�ֱ�����������ô�����ı�locale�ģ�
    sub_404ed0_LogFormat(
        LOG_TAG,
        "Sorry, not implemented yet! Using Default Locale"
    );
    return TRUE;
}