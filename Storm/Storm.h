#pragma once

#include <Windows.h>

// ʹ�� Storm �ĵط���Ҫ�ȶ��� USE_STORM_API Ȼ���ٰ�����ͷ�ļ���
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