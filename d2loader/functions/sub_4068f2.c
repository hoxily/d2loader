#include "pch.h"
#include <stdio.h>
#include <string.h>
#include "sub_4068f2.h"
#include "../global-variables.h"
#include "sub_406887.h"

BOOL sub_4068f2_LoadConfFile(
    const char* filename
)
{
    // Buf �� var_27FF ������ͬһ��������var_27FF �ȼ��� Buf + 1
    char buffer[0x2800];

    // call __alloca_probe ��������ջ�������˴��� PAGE_SIZE ��С�Ŀռ䣬���±�������������μ����롣
    // ����ȷ���������е�����ʱ��ջ�Ŀռ��㹻������� https://bbs.pediy.com/thread-145752.htm
    // �˴����Ǻ�������

    if (filename == NULL)
    {
        return FALSE;
    }

    if (filename[0] == '\0')
    {
        return FALSE;
    }

    FILE* ebx_fp = fopen(filename, "r");
    if (ebx_fp == NULL)
    {
        return FALSE;
    }

    const size_t edi_bufferSize = sizeof(buffer);

    // fgets(buffer, edi_bufferSize, ebx_fp) �����ڻ���ﱻ��ֳ���������
    while (fgets(buffer, edi_bufferSize, ebx_fp))
    {
        // fgets �Ὣ \n �ַ�Ҳ���� buffer �С�
        int eax_i = strlen(buffer);
        // dec eax ָ�� ��Ӱ�� CF ��־λ������ OF, SF, ZF, AF, PF ��λ������ݼ�������Ӧ���á�
        // js ָ���ʾjump short if sign (SF=1)��
        while (--eax_i >= 0)
        {
            if (buffer[eax_i] == '\r' || buffer[eax_i] == '\n')
            {
                continue;
            }
            else
            {
                break;
            }
        }

        buffer[eax_i + 1] = '\0';

        sub_406887_ProcessCommandLineArguments(buffer);
    }

    fclose(ebx_fp);

    if (global_dd_408620_settings->db_07ae_removeConfFile.value)
    {
        _unlink(filename);
    }

    return TRUE;
}
