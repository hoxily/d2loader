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

/*
* ���ַ���������صı������е�һ�����
*/
struct string_array
{
    /*
    * ���ݴ洢��
    */
    union string_array_item* data;
    /*
    * ��ǰԪ�ظ���
    */
    int length;
    /*
    * ���Ͻ�β������ǵ�Ԫ�ظ������ȼ��� length + 1
    */
    int lengthWithSentinel;
    /*
    * �洢����Ԫ������
    */
    int capacity;
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

    int var_10 = STRING_ARRAY_INIT_ELEMENT_SIZE;
    struct string_array ebx_stringArray;
    ebx_stringArray.data = (union string_array_item*)malloc(sizeof(char*) * STRING_ARRAY_INIT_ELEMENT_SIZE);
    if (ebx_stringArray.data == NULL)
    {
        free(buffer);
        return NULL;
    }
    ebx_stringArray.length = 0;
    ebx_stringArray.data[0].offset = 0;
    char* esi_ptr = buffer;
    BOOL var_8 = TRUE;
    const char* edi_ptr = pattern;
    if (*edi_ptr != '\0')
    {
        ebx_stringArray.capacity = STRING_ARRAY_INIT_ELEMENT_SIZE;
        // ����NULL������ʶ������£��ַ�������ĳ��ȡ�
        ebx_stringArray.lengthWithSentinel = 1;
        for (; *edi_ptr != '\0'; edi_ptr++)
        {
            BOOL findAnySeparator = FALSE;
            // ���ﲢ��������pattern������s������s�е��κε����ַ����Ƿָ����
            for (const char* edx_ptr = s; *edx_ptr != '\0'; edx_ptr++)
            {
                if (*edx_ptr == *edi_ptr)
                {
                    if (!var_8)
                    {
                        *esi_ptr = '\0';
                        esi_ptr++;
                        ebx_stringArray.length++;
                        ebx_stringArray.lengthWithSentinel++;
                        var_8 = TRUE;
                    }
                    findAnySeparator = TRUE;
                }
            }

            if (!findAnySeparator)
            {
                if (var_8)
                {
                    if (ebx_stringArray.lengthWithSentinel >= var_10)
                    {
                        var_10 += STRING_ARRAY_INCREMENT;
                        ebx_stringArray.capacity += STRING_ARRAY_INCREMENT;
                        union string_array_item* tmp = (union string_array_item*)realloc(
                            ebx_stringArray.data,
                            ebx_stringArray.capacity * sizeof(char*)
                        );
                        if (tmp == NULL)
                        {
                            free(ebx_stringArray.data);
                            free(buffer);
                            return NULL;
                        }
                        ebx_stringArray.data = tmp;
                    }

                    var_8 = FALSE;
                    ebx_stringArray.data[ebx_stringArray.length].offset = esi_ptr - buffer;
                }

                *esi_ptr = *edi_ptr;
                esi_ptr++;
            }
        }

        if (!var_8)
        {
            *esi_ptr = '\0';
            esi_ptr++;
            ebx_stringArray.length++;
        }
    }
    /*
    * ����һ�����ڴ棬�ֱ���
    * 1. char* ���飻
    * 2. ���������ʶ NULL��
    * 3. char* ������ָ����ַ������ݣ�
    */
    size_t pointerBlockSize = sizeof(char*) * (ebx_stringArray.length + 1);
    size_t stringDataBlockSize = esi_ptr - buffer;
    char* resultBuffer = (char*)malloc(pointerBlockSize + stringDataBlockSize);
    if (resultBuffer == NULL)
    {
        free(buffer);
        free(ebx_stringArray.data);
        return NULL;
    }

    memcpy(
        resultBuffer + pointerBlockSize,
        buffer,
        stringDataBlockSize
    );

    for (int i = 0; i < ebx_stringArray.length; i++)
    {
        ebx_stringArray.data[i].pointer = ebx_stringArray.data[i].offset + resultBuffer + pointerBlockSize;
    }
    ebx_stringArray.data[ebx_stringArray.length].pointer = NULL;

    memcpy(
        resultBuffer,
        ebx_stringArray.data,
        pointerBlockSize
    );

    free(buffer);
    free(ebx_stringArray.data);
    if (count != NULL)
    {
        *count = ebx_stringArray.length;
    }
    return (char**)resultBuffer;
}