#include "pch.h"
#include "sub_4076ab.h"
#include <assert.h>

DWORD sub_4076ab_GetImageBase(
    HMODULE hModule
)
{
    IMAGE_DOS_HEADER* dosHeader = (IMAGE_DOS_HEADER*)hModule;
    if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE)
    {
        return 0;
    }

    assert(offsetof(IMAGE_DOS_HEADER, e_lfanew) == 0x3c);
    IMAGE_NT_HEADERS32* ntHeader = (IMAGE_NT_HEADERS32*)((char*)dosHeader + dosHeader->e_lfanew);

    assert(offsetof(IMAGE_NT_HEADERS32, Signature) == 0);
    if (ntHeader->Signature != IMAGE_NT_SIGNATURE)
    {
        return 0;
    }

    assert(offsetof(IMAGE_NT_HEADERS32, OptionalHeader) + offsetof(IMAGE_OPTIONAL_HEADER32, ImageBase) == 0x34);
    return ntHeader->OptionalHeader.ImageBase;
}