#include "sub_405c59.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

int sub_405c59_ParseHexByteString(
    const char* str,
    int* buffer
)
{
    char dest[4];
    int byteCount = 0;
    
    const char* esi_ptr = str;
    int strLength = strlen(str);
    char* endPtr = NULL;
    
    if (strLength <= 0)
    {
        return 0;
    }

    int* edi_ptr = buffer;
    for (int i = 0; i < strLength; i++, esi_ptr++)
    {
        assert(' ' == 0x20);
        assert('\t' == 0x9);
        if (*esi_ptr == ' ' || *esi_ptr == '\t')
        {
            continue;
        }

        dest[2] = '\0';
        strncpy(&dest[0], esi_ptr, 2);
        // 用于消除警告： C6053 对“strncpy”的前一调用可能没有为字符串“dest”添加字符串零终止符。
        dest[2] = '\0';

        unsigned long value = strtoul(dest, &endPtr, 16);
        if (endPtr != NULL && endPtr[0] != '\0')
        {
            *edi_ptr = 0;
        }
        else
        {
            value = 0xff00 | (value & 0xff);
            *edi_ptr = value;
        }

        byteCount++;
        edi_ptr++;
        i++;
    }
    return byteCount;
}