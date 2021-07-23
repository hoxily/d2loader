#include "pch.h"
#include "sub_4054c2.h"
#include "../global-variables.h"
#include "sub_40513a.h"

HMODULE WINAPI sub_4054c2_my_LoadLibraryA(
    LPCSTR lpLibFileName
)
{
    HMODULE hModule = NULL;
    if (global_dd_4085a0_LoadLibraryA == NULL)
    {
        hModule = LoadLibraryA(lpLibFileName);
    }
    else
    {
        hModule = global_dd_4085a0_LoadLibraryA(lpLibFileName);
    }

    if (hModule != NULL)
    {
        sub_40513a(
            hModule,
            "Kernel32.dll",
            global_dd_4023f0,
            NULL,
            NULL
        );
    }

    return hModule;
}
