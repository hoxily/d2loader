#pragma once

#include <Windows.h>

// 使用 Storm 的地方需要先定义 USE_STORM_API 然后再包含此头文件。
#ifdef USE_STORM_API
    #define STORM_API __declspec(dllimport)
#else
    #define STORM_API 
#endif

STORM_API BOOL __stdcall dll_storm_266(
    const char* mpqFile,
    DWORD flag1,
    DWORD flag2,
    _Out_ DWORD* mpqHandle
);