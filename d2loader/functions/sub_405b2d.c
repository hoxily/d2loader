#include "sub_405b2d.h"

BOOL sub_405b2d_IncreasePPtr(
    void** pBaseAddress,
    ptrdiff_t increment
)
{
    char** ptr = (char**)pBaseAddress;
    *ptr += increment;

    return TRUE;
}