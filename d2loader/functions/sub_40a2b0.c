#include "pch.h"
#include "sub_40a2b0.h"
#include "sub_40a360.h"

extern char* sub_40a2b0_GetBnCacheFileName(
    char* buffer
)
{
    DWORD pid = sub_40a360_GetCurrentProcessId();
    wsprintfA(buffer, "./bncache-%u.dat", pid);
    return buffer;
}