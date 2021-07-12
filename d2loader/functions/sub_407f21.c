#include "sub_407f21.h"
#include <assert.h>

/*
* �ظ�ʹ�õ��ַ���ָ������
*/
union string_array_item
{
    /*
    * һ��ʼ��¼���Ǹ��ַ�����buffer�ϵ�ƫ����
    */
    ptrdiff_t offset;
    /*
    * ���淵�ؽ��ʱ��ת����������charָ�롣
    */
    char* pointer;
};

// ÿ������ʱ���ӵ�Ԫ������
#define STRING_ARRAY_INCREMENT 0x20
// ��ʼԪ������
#define STRING_ARRAY_INIT_ELEMENT_SIZE 0x20

char** sub_407f21_SplitString(
    const char* pattern,
    const char* s,
    DWORD* count
)
{
    // fix by hoxily@qq.com, ��ȫ�������Ҫ�� count ��Ϊ 0
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
    // ����NULL������ʶ������£��ַ�������ĳ��ȡ�
    unsigned int stringArrayLengthWithSentinel = 1;
    for (; *edi_ptr != '\0'; edi_ptr++)
    {
        // ���ﲢ��������pattern������s������s�е��κε����ַ����Ƿָ����
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
        ;// ����ǩ����û�����ʱ����Ҫ���һ������䣬������뱨��
    }

    if (!var_8)
    {
        *esi_ptr = '\0';
        esi_ptr++;
        stringArrayLength++;
    }

constructResult:
    /*
    * ����һ�����ڴ棬�ֱ���
    * 1. char* ���飻
    * 2. ���������ʶ NULL��
    * 3. char* ������ָ����ַ������ݣ�
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