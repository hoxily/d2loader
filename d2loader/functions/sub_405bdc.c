#include "pch.h"
#include "sub_405bdc.h"
#include "sub_405c29.h"

void* sub_405bdc_SearchByteSequence(
    HMODULE hModule,
    DWORD sizeOfImage,
    DWORD* byteSequence,
    int byteCount
)
{
    if (sizeOfImage == 0 || byteCount == 0)
    {
        return NULL;
    }

    // 汇编中 sizeOfImage 被挪用。此处定义新变量。
    int searchCount = sizeOfImage - byteCount;
    if (searchCount <= 0)
    {
        return NULL;
    }

    for (int esi_i = 0; esi_i < searchCount; esi_i++)
    {
        BYTE* startAddress = (BYTE*)hModule + esi_i;
        BOOL match = sub_405c29_IsByteSequenceMatch(
            startAddress,
            byteSequence,
            byteCount
        );
        if (match)
        {
            return startAddress;
        }
    }
    return NULL;
}