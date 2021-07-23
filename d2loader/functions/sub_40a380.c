#include "pch.h"
#include "sub_40a380.h"
#include "../data-types.h"
#include <assert.h>
#include "sub_404ed0.h"

void sub_40a380(
)
{
    DWORD oldProtect;
    VirtualProtect(
        (void*)0x6ff333b9,
        0x5c7,
        PAGE_EXECUTE_READWRITE,
        &oldProtect
    );

    *(DWORD*)0x6FF333B9 = 0x904D6F63;
    *(DWORD*)0x6FF334B9 = 0x904D6E63;
    *(DWORD*)0x6FF33960 = 0x904D69BC;
    *(DWORD*)0x6FF3397B = 0x904D69A1;

    DWORD unused;
    VirtualProtect(
        (void*)0x6ff333b9,
        0x5c7,
        oldProtect,
        &unused
    );

    FlushInstructionCacheAfterModifyCode();
}

void FlushInstructionCacheAfterModifyCode(
)
{
    HMODULE moduleKernel32 = GetModuleHandleA("KERNEL32.DLL");
    assert(moduleKernel32 != NULL);
    fn_FlushInstructionCache fnFlushInstructionCache = (fn_FlushInstructionCache)GetProcAddress(
        moduleKernel32,
        "FlushInstructionCache"
    );
    assert(fnFlushInstructionCache != NULL);
    BOOL ret = fnFlushInstructionCache(
        GetCurrentProcess(),
        NULL,
        0
    );
    sub_404ed0_LogFormat(
        LOG_TAG,
        "FlushInstructionCache ret: %d",
        ret
    );
}