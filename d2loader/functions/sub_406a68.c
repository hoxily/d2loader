#include <stddef.h>
#include <Windows.h>
#include "sub_406a68.h"

const char* sub_406a68_CutFirstArgument(
    const char* args,
    char* buffer
)
{
    if (args == NULL)
    {
        return NULL;
    }

    const char* eax_p = args;
    char* edx_pBuffer = buffer;
    BOOL esi_isDoubleQuoteMode = FALSE;

    // �ڻ����whileѭ�������һ����������ת��*eax_p != '\0' ��Ӧ�Ĵ��뱻�ظ���һ�顣
    while (*eax_p != '\0')
    {
        if (*eax_p == '"')
        {
            /* xor ecx, ecx
             * test esi, esi
             * setz cl
             * mov esi, ecx
             * setz ����˼�����ZF��־λΪ1����cl��Ϊ1��������Ϊ0��
             * Ҳ������� esi Ϊ0����esi��Ϊ1������esi��Ϊ0���൱��ȡ�����㣻
             * ������������ת��
             * */
            esi_isDoubleQuoteMode = !esi_isDoubleQuoteMode;
        }
        else
        {
            if (*eax_p == ' ' && !esi_isDoubleQuoteMode)
            {
                break;
            }

            // �������е� \" ����Ϊ�ַ������ݶԴ�����������Ϊ���пո���ַ����߽綨����Դ���
            if (*eax_p == '\\' && eax_p[1] == '"')
            {
                eax_p = eax_p + 1;
            }

            // ���������Ч����
            *edx_pBuffer = *eax_p;
            edx_pBuffer++;
        }
        eax_p++;
    }

    *edx_pBuffer = '\0';

    // �Ƴ������в���֮�����ڷָ��Ŀո��ַ�
    while (*eax_p == ' ')
    {
        eax_p++;
    }

    return eax_p;
}
