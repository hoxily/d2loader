#include "sub_407f21.h"
#include <assert.h>

/*
* 重复使用的字符串指针数组
*/
union string_array_item
{
    /*
    * 一开始记录的是该字符串在buffer上的偏移量
    */
    ptrdiff_t offset;
    /*
    * 后面返回结果时，转换成真正的char指针。
    */
    char* pointer;
};

// 每次扩容时增加的元素数量
#define STRING_ARRAY_INCREMENT 0x20
// 初始元素数量
#define STRING_ARRAY_INIT_ELEMENT_SIZE 0x20

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

    size_t var_10 = STRING_ARRAY_INIT_ELEMENT_SIZE;
    union string_array_item* ebx_stringArray = (union string_array_item*)malloc(sizeof(char*) * STRING_ARRAY_INIT_ELEMENT_SIZE);
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
    size_t stringArrayCapacity = STRING_ARRAY_INIT_ELEMENT_SIZE;
    // 包括NULL结束标识的情况下，字符串数组的长度。
    unsigned int stringArrayLengthWithSentinel = 1;
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
                    stringArrayLengthWithSentinel++;
                    var_8 = TRUE;
                }
                goto breakToOutterLoopEnd;;
            }
        }

        if (var_8)
        {
            if (stringArrayLengthWithSentinel >= var_10)
            {
                var_10 += STRING_ARRAY_INCREMENT;
                stringArrayCapacity += STRING_ARRAY_INCREMENT;
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
        ;// 当标签后面没有语句时，需要添加一个空语句，否则编译报错。
    }

    if (!var_8)
    {
        *esi_ptr = '\0';
        esi_ptr++;
        stringArrayLength++;
    }

constructResult:
    /*
    * 分配一整块内存，分别存放
    * 1. char* 数组；
    * 2. 数组结束标识 NULL；
    * 3. char* 数组针指向的字符串数据；
    */
    size_t pointerBlockSize = sizeof(char*) * (stringArrayLength + 1);
    size_t stringDataBlockSize = esi_ptr - buffer;
    char* resultBuffer = (char*)malloc(pointerBlockSize + stringDataBlockSize);
    if (resultBuffer == NULL)
    {
        free(buffer);
        free(ebx_stringArray);
        return NULL;
    }

    memcpy(
        resultBuffer + pointerBlockSize,
        buffer,
        stringDataBlockSize
    );

    for (int i = 0; i < stringArrayLength; i++)
    {
        ebx_stringArray[i].pointer = ebx_stringArray[i].offset + resultBuffer + pointerBlockSize;
    }
    ebx_stringArray[stringArrayLength].pointer = NULL;

    memcpy(
        resultBuffer,
        ebx_stringArray,
        pointerBlockSize
    );

    free(buffer);
    free(ebx_stringArray);
    if (count != NULL)
    {
        *count = stringArrayLength;
    }
    return (char**)resultBuffer;
}