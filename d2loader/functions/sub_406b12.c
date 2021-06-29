#include <stddef.h>
#include <assert.h>
#include "sub_406b12.h"
#include "../global-variables.h"
#include "../constants.h"

BOOL sub_406b12_StoreCommandLineSetting(
    int i,
    char* buffer
)
{
    assert(offsetof(struct string_index_item, type) == 0x10);
    DWORD type = global_dd_402ea8_CommandLineArgumentTable[i].type;
    DWORD offset = global_dd_402ea8_CommandLineArgumentTable[i].offset;
    if (type == ARG_TYPE_NO_PARAM)
    {
        BYTE toggle = global_dd_408620_settings->padding[offset];
        // 原代码是 xor byte ptr [eax], 1
        // 看起来会翻转原来已经设置好的配置项。
        // 预测：d2loader.exe -w -w  这样的启动参数，并不会窗口化运行，而是全屏运行。
        // 实测结果：确实是没有窗口化运行。
        global_dd_408620_settings->padding[offset] = !toggle;
    }
    else if (type == ARG_TYPE_UNSIGNED_SHORT_INT)
    {
        /*
        * 如果 base(_Radix) 为0，则使用 strSource 指向的字符串的初始字符来确定基。
        * 如果第一个字符为 0，且第二个字符不为“x”或“X”，则将该字符串视为八进制整数。
        * 如果第一个字符为“0”，且第二个字符为“x”或“X”，则将该字符串视为十六进制整数。
        * 如果第一个字符是“1”至“9”，则将该字符串视为十进制整数。
        */
        WORD ax = (WORD)strtoul(buffer, NULL, 0);
        WORD* ptr = (WORD*)(global_dd_408620_settings->padding + offset);
        *ptr = ax;
    }
    else if (type == ARG_TYPE_UNSIGNED_INT)
    {
        DWORD eax = strtoul(buffer, NULL, 0);
        DWORD* ptr = (DWORD*)(global_dd_408620_settings->padding + offset);
        *ptr = eax;
    }
    else
    {
        // 此时 type 表示字符串缓冲区的大小。
        int settingBufferSize = type;
        // 使用 (void) 丢弃 lstrcpynA 的返回结果。否则Visual Studio会有warning。
        (void)lstrcpynA(global_dd_408620_settings->padding + offset, buffer, settingBufferSize);
        // 强制设置缓冲区的最后一个字符为null结束符，以免溢出。
        global_dd_408620_settings->padding[offset + settingBufferSize - 1] = '\0';
    }

    return TRUE;
}
