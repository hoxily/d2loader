#include "pch.h"
#include "sub_4076ca.h"
#include <assert.h>

DWORD sub_4076ca_GetSizeOfImage(
    HMODULE hModule
)
{
    IMAGE_DOS_HEADER* dosHeader = (IMAGE_DOS_HEADER*)hModule;
    assert(offsetof(IMAGE_DOS_HEADER, e_magic) == 0);
    if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE)
    {
        return 0;
    }

    assert(offsetof(IMAGE_DOS_HEADER, e_lfanew) == 0x3c);
    IMAGE_NT_HEADERS32* ntHeader = (IMAGE_NT_HEADERS32*)((char*)hModule + dosHeader->e_lfanew);
    if (ntHeader->Signature != IMAGE_NT_SIGNATURE)
    {
        return 0;
    }

    assert(offsetof(IMAGE_NT_HEADERS32, OptionalHeader) + offsetof(IMAGE_OPTIONAL_HEADER32, SizeOfImage) == 0x50);
    return ntHeader->OptionalHeader.SizeOfImage;
}