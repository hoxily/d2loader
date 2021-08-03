#pragma once

#include "pch.h"
#include "StormApi.h"

#ifdef __cplusplus
extern "C" {
#endif

    STORM_API BOOL __stdcall storm_266(
        const char* mpqFile,
        DWORD flag1,
        DWORD flag2,
        _Out_ DWORD* mpqHandle
    );

#ifdef __cplusplus
}
#endif