#include "pch.h"
#include <assert.h>
#include "sub_4053b3.h"

BOOL sub_4053b3_IsWin32NtPlatform(
)
{
    OSVERSIONINFOA version;
    memset(&version, 0, sizeof(version));
    assert(offsetof(OSVERSIONINFOA, dwOSVersionInfoSize) == 0);
    version.dwOSVersionInfoSize = sizeof(version);

    // ¡Ÿ ±Ω˚”√ obsolete°£∑Ò‘Úvisual studio±‡“Î ß∞‹°£
#pragma warning(disable: 4996)
    BOOL ret = GetVersionExA(&version);
#pragma warning(default: 4996)

    if (ret)
    {
        assert(offsetof(OSVERSIONINFOA, dwPlatformId) == 0x10);
        if (version.dwPlatformId == VER_PLATFORM_WIN32_NT)
        {
            return TRUE;
        }
    }
    return FALSE;
}
