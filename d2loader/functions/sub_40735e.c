#include "pch.h"
#include "sub_40735e.h"
#include "../constant-strings.h"
#include "sub_407380.h"
#include "sub_404ed0.h"

BOOL sub_40735e_CheckExpansion()
{
    if (!sub_407380_CheckFileExist(CSTR_D2_EXP_DOT_MPQ))
    {
        sub_404ed0_LogFormat(
            LOG_TAG,
            "%s not exist.\n",
            CSTR_D2_EXP_DOT_MPQ);
        return FALSE;
    }
    if (!sub_407380_CheckFileExist(CSTR_D2_X_TALK_DOT_MPQ))
    {
        sub_404ed0_LogFormat(
            LOG_TAG,
            "%s not exist.\n",
            CSTR_D2_X_TALK_DOT_MPQ);
        return FALSE;
    }
    /* ����Ļ�����Ƚ��Ѷ���
    * push filePath
    * call CheckFileExist
    * neg eax
    * sbb eax, eax
    * pop ecx ;__cdecl ����Լ�����÷���ƽ��ջ�����Ի� add esp, xxx ���������ɸ� pop ecx�ķ�ʽ��ƽ��ջ��
    * neg eax
    *
    * neg eax ����˼�� eax <- (0 - eax)����eax��0ʱ��CF��־λ��λ��
    * sbb�Ǵ���λ�ļ������� eax <- (eax - eax - CF)��
    * ���Դ�eax�Ƿ�Ϊ0�����Ƿ�0���ֱ����ۡ�
    */
    return TRUE;
}
