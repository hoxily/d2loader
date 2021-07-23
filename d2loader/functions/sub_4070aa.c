#include "pch.h"
#include "sub_4070aa.h"
#include "sub_404ed0.h"

HMODULE sub_4070aa_D2LoadLibrary(
    const char* dllFileName
)
{
    HMODULE hModule = LoadLibraryA(dllFileName);
    if (hModule == NULL)
    {
        sub_404ed0_LogFormat(
            LOG_TAG,
            "Error Loading %s",
            dllFileName
        );
    }
    return hModule;
}