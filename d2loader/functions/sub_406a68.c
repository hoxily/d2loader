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

    // 在汇编里while循环被拆出一个单独的跳转，*eax_p != '\0' 对应的代码被重复了一遍。
    while (*eax_p != '\0')
    {
        if (*eax_p == '"')
        {
            /* xor ecx, ecx
             * test esi, esi
             * setz cl
             * mov esi, ecx
             * setz 的意思是如果ZF标志位为1，则将cl置为1，否则置为0；
             * 也就是如果 esi 为0，则esi变为1；否则esi变为0；相当于取非运算；
             * 避免了条件跳转。
             * */
            esi_isDoubleQuoteMode = !esi_isDoubleQuoteMode;
        }
        else
        {
            if (*eax_p == ' ' && !esi_isDoubleQuoteMode)
            {
                break;
            }

            // 命令行中的 \" 会作为字符串内容对待，而不是作为含有空格的字符串边界定界符对待。
            if (*eax_p == '\\' && eax_p[1] == '"')
            {
                eax_p = eax_p + 1;
            }

            // 拷贝这个有效内容
            *edx_pBuffer = *eax_p;
            edx_pBuffer++;
        }
        eax_p++;
    }

    *edx_pBuffer = '\0';

    // 移除命令行参数之间用于分隔的空格字符
    while (*eax_p == ' ')
    {
        eax_p++;
    }

    return eax_p;
}
