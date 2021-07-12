#include "sub_405b3b.h"
#include <assert.h>
#include "sub_4076ca.h"
#include "sub_407f21.h"
#include "sub_405c59.h"
#include "sub_405bdc.h"

void* sub_405b3b_SearchPattern(
    HMODULE hModule,
    const char* pattern
)
{
    if (hModule == NULL || pattern == NULL)
    {
        return NULL;
    }

    DWORD sizeOfImage = sub_4076ca_GetSizeOfImage(hModule);
    DWORD count;
    char** esi_ptr = sub_407f21_SplitString(pattern, ",", &count);
    if (count != 2)
    {
        free(esi_ptr);
        return NULL;
    }

    // 汇编代码中对 pattern 实参进行了复用。我们重新定义一个变量
    unsigned long offset = strtoul(esi_ptr[0], NULL, 0);
    size_t len = strlen(esi_ptr[1]);
    len >>= 1;
    len <<= 2;
    void* buffer = malloc(len);
    assert(buffer != NULL);
    int count2 = sub_405c59_ParseHexByteString(esi_ptr[1], buffer);
    free(esi_ptr);
    void* address = sub_405bdc(hModule, sizeOfImage, buffer, count2);
    free(buffer);

    if (address != NULL)
    {
        return (char*)address + offset;
    }
    else
    {
        return NULL;
    }
}