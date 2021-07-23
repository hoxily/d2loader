#include "pch.h"
#include "sub_40a360.h"
#include <assert.h>

typedef DWORD (WINAPI *fn_GetCurrentProcessId)(
    void
);

DWORD sub_40a360_GetCurrentProcessId(
)
{
    HMODULE hModule = GetModuleHandleA("KERNEL32.DLL");
    assert(hModule != NULL);
    fn_GetCurrentProcessId fn = (fn_GetCurrentProcessId)GetProcAddress(hModule, "GetCurrentProcessId");
    assert(fn != NULL);
    return fn();
}