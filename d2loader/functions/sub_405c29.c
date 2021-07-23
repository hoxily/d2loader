#include "pch.h"
#include "sub_405c29.h"

BOOL sub_405c29_IsByteSequenceMatch(
    const BYTE* startAddress,
    DWORD* byteSequence,
    int byteCount
)
{
    DWORD* eax_ptr = byteSequence;
    for (int ecx_i = 0; ecx_i < byteCount; ecx_i++, eax_ptr++)
    {
        BYTE byte = (*eax_ptr & 0xff);
        DWORD mask = (*eax_ptr & 0xff00) >> 8;
        BYTE value = mask & startAddress[ecx_i];
        if (value != byte)
        {
            return FALSE;
        }
    }

    return TRUE;
}