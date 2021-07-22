#include "sub_40a600.h"
#include "sub_40a2b0.h"
#include "sub_404ed0.h"
#include <Windows.h>
#include <stdio.h>

void sub_40a600_CleanupBnCacheFile(
)
{
    char fileName[0x104];
    // ½è eax ¼Ä´æÆ÷´«µÝ²ÎÊý
    sub_40a2b0_GetBnCacheFileName(fileName);
    HANDLE handle = (HANDLE)0x6ff3dce8;
    if (handle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(handle);
    }
    int errorCode = _unlink(fileName);
    if (errorCode == -1)
    {
        sub_404ed0_LogFormat(
            LOG_TAG,
            "Failed to delete BnCache file: %s",
            fileName
        );
    }
}
