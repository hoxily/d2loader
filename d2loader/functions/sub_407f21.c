#include "sub_407f21.h"

char** sub_407f21_SplitString(
    const char* pattern,
    const char* s,
    DWORD* count
)
{
    // fix by hoxily@qq.com, 安全起见，需要将 count 置为 0
    *count = 0;

    if (pattern == NULL || s == NULL)
    {
        return NULL;
    }

    char* buffer = (char*)malloc(strlen(pattern) + 1);
    if (buffer == NULL)
    {
        return NULL;
    }

    size_t var_10 = 0x20;
    char** ebx_stringArray = malloc(0x80);
    if (ebx_stringArray == NULL)
    {
        free(buffer);
        return NULL;
    }
    int stringArrayLength = 0;
    ebx_stringArray[0] = NULL;
    char* esi_ptr = buffer;
    BOOL var_8 = TRUE;
    size_t stringArrayCapacity = 0x80 / sizeof(char*);
    unsigned int var_c = 1;
    for (const char* edi_ptr = pattern; *edi_ptr != '\0'; edi_ptr++)
    {
        // 这里并不是想在pattern中搜索s，而是s中的任何单个字符都是分割符。
        for (const char* edx_ptr = s; *edx_ptr != '\0'; edx_ptr++)
        {
            if (*edx_ptr == *edi_ptr)
            {
                if (var_8)
                {
                    *esi_ptr = '\0';
                    esi_ptr++;
                    stringArrayLength++;
                    var_c++;
                    var_8 = TRUE;
                }
                break;
            }
        }

        if (var_8)
        {
            if (var_c >= var_10)
            {
                var_10 += 0x20;
                stringArrayCapacity += 0x80 / sizeof(char*);
                char** tmp = (char**)realloc(ebx_stringArray, stringArrayCapacity * sizeof(char*));
                if (tmp == NULL)
                {
                    free(ebx_stringArray);
                    free(buffer);
                    return NULL;
                }
                ebx_stringArray = tmp;
            }
        }
    }

    return NULL;//TODO
}