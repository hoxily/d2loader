#include "pch.h"
#include "sub_40aaf0.h"
#include "sub_40a380.h"


extern void sub_40aaf0(
)
{
    DWORD oldProtect;
    VirtualProtect(
        (void*)0x6FAF5564,
        0x20,
        PAGE_EXECUTE_READWRITE,
        &oldProtect
    );

    *(DWORD*)0x6FAF5565 = 0x90915612;
    *(BYTE*)0x6FAF5564 = 0xe9;

    DWORD unused;
    VirtualProtect(
        (void*)0x6FAF5564,
        0x20,
        oldProtect,
        &unused
    );

    FlushInstructionCacheAfterModifyCode();
}