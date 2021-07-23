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
    // Buf 与 var_27FF 好像是同一个变量。var_27FF 等价于 Buf + 1
    char buffer[0x2800];

    // call __alloca_probe 是由于在栈上申请了大于 PAGE_SIZE 大小的空间，导致编译器插入了这段检查代码。
    // 用以确保程序运行到这里时，栈的空间足够。具体见 https://bbs.pediy.com/thread-145752.htm
    // 此处我们忽略它。

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

    // fgets(buffer, edi_bufferSize, ebx_fp) 调用在汇编里被拆分成了两处。
    while (fgets(buffer, edi_bufferSize, ebx_fp))
    {
        // fgets 会将 \n 字符也读入 buffer 中。
        int eax_i = strlen(buffer);
        // dec eax 指令 不影响 CF 标志位。其余 OF, SF, ZF, AF, PF 标位都会根据计算结果相应设置。
        // js 指令表示jump short if sign (SF=1)。
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
