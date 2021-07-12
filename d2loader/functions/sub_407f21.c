#include "sub_407f21.h"
#include <assert.h>

union string_array_item
{
    char* pointer;
    ptrdiff_t offset;
};

char** sub_407f21_SplitString(
    const char* pattern,
    const char* s,
    DWORD* count
)
{
    // fix by hoxily@qq.com, 安全起见，需要将 count 置为 0
    if (count != NULL)
    {
        *count = 0;
    }
    
    assert(sizeof(union string_array_item) == sizeof(char*));
    assert(sizeof(union string_array_item) == sizeof(ptrdiff_t));

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
    union string_array_item* ebx_stringArray = malloc(0x80);
    if (ebx_stringArray == NULL)
    {
        free(buffer);
        return NULL;
    }
    int stringArrayLength = 0;
    ebx_stringArray[0].offset = 0;
    char* esi_ptr = buffer;
    BOOL var_8 = TRUE;
    const char* edi_ptr = pattern;
    if (*edi_ptr == '\0')
    {
        goto constructResult;
    }
    size_t stringArrayCapacity = 0x80 / sizeof(char*);
    unsigned int var_c = 1;
    for (; *edi_ptr != '\0'; edi_ptr++)
    {
        // 这里并不是想在pattern中搜索s，而是s中的任何单个字符都是分割符。
        for (const char* edx_ptr = s; *edx_ptr != '\0'; edx_ptr++)
        {
            if (*edx_ptr == *edi_ptr)
            {
                if (!var_8)
                {
                    *esi_ptr = '\0';
                    esi_ptr++;
                    stringArrayLength++;
                    var_c++;
                    var_8 = TRUE;
                }
                goto breakToOutterLoopEnd;;
            }
        }

        if (var_8)
        {
            if (var_c >= var_10)
            {
                var_10 += 0x20;
                stringArrayCapacity += 0x80 / sizeof(char*);
                union string_array_item* tmp = (union string_array_item*)realloc(ebx_stringArray, stringArrayCapacity * sizeof(char*));
                if (tmp == NULL)
                {
                    free(ebx_stringArray);
                    free(buffer);
                    return NULL;
                }
                ebx_stringArray = tmp;
            }

            var_8 = FALSE;
            ebx_stringArray[stringArrayLength].offset = esi_ptr - buffer;
        }

        *esi_ptr = *edi_ptr;
        esi_ptr++;

    breakToOutterLoopEnd:
        ;// 需要一个空语句，否则编译报错。
    }

    if (!var_8)
    {
        *esi_ptr = '\0';
        esi_ptr++;
        stringArrayLength++;
    }

    constructResult:
    size_t size = sizeof(char*) * (stringArrayLength + 1) + (esi_ptr - buffer);
    char* resultBuffer = (char*)malloc(size);
    if (resultBuffer == NULL)
    {
        free(buffer);
        free(ebx_stringArray);
        return NULL;
    }

    memcpy(
        resultBuffer + sizeof(char*) * (stringArrayLength + 1),
        buffer,
        esi_ptr - buffer
    );

    for (int i = 0; i < stringArrayLength; i++)
    {
        ebx_stringArray[i].pointer = ebx_stringArray[i].offset + resultBuffer + sizeof(char*) * (stringArrayLength + 1);
    }
    ebx_stringArray[stringArrayLength * sizeof(char*)].pointer = NULL;
    memcpy(
        resultBuffer,
        ebx_stringArray,
        (stringArrayLength + 1) * sizeof(char*)
    );
    free(buffer);
    free(ebx_stringArray);
    if (count != NULL)
    {
        *count = stringArrayLength;
    }
    return (char**)resultBuffer;
}