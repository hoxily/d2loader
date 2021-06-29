#include <assert.h>
#include "sub_40a440.h"
#include "../data-types.h"

UINT sub_40a440(
)
{
    HMODULE hModule = GetModuleHandleA("KERNEL32.DLL");
    assert(hModule != NULL);
    fn_SetErrorMode fn = (fn_SetErrorMode)GetProcAddress(hModule, "SetErrorMode");
    return fn(SEM_FAILCRITICALERRORS);
}