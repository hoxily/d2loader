#include "sub_405b3b.h"
#include "sub_4076ca.h"
#include "sub_407f21.h"

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
    char** esi_ptr = sub_407f21(pattern, ",", &count);
    if (count != 2)
    {
        free(esi_ptr);
        return NULL;
    }

    strtoul(esi_ptr[0], NULL, 0);
    return NULL;//TODO
}