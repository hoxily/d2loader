#pragma once

#include "data-types-forward-structs.h"

// fn_OnInitPlugin 函数的栈平衡是函数内部做的。
typedef void(__stdcall* fn_OnInitPlugin)(DWORD gameProductVersionFlag);

// fn_RunPlugin 函数的栈平衡是函数内部做的。
typedef BOOL(__stdcall* fn_RunPlugin)(DWORD reasonFlag, DWORD* gameProductVersionFlag);

// 由于不传参数，所以 __stdcall 与 __cdecl 一致。
typedef struct query_interface_result* (__stdcall* fn_QueryInterface)();

typedef HMODULE(WINAPI* fn_LoadLibraryA)(_In_ LPCSTR lpLibFileName);

typedef DWORD(WINAPI* fn_GetModuleFileNameA)(
    _In_opt_ HMODULE hModule,
    _Out_ LPSTR lpFilename,
    _In_ DWORD nSize);

typedef HWND(WINAPI* fn_FindWindowA)(
    _In_opt_ LPCSTR lpClassName,
    _In_opt_ LPCSTR lpWindowName
    );

typedef UINT(WINAPI* fn_SetErrorMode)(
    _In_ UINT uMode
    );

typedef int(WINAPI* fn_MultiByteToWideChar)(
    _In_ UINT CodePage,
    _In_ DWORD dwFlags,
    _In_NLS_string_(cbMultiByte) LPCCH lpMultiByteStr,
    _In_ int cbMultiByte,
    _Out_writes_to_opt_(cchWideChar, return) LPWSTR lpWideCharStr,
    _In_ int cchWideChar
    );

typedef wchar_t* (__cdecl* fn_wcsrchr)(
    const wchar_t* _Str,
    wchar_t _Ch
    );

typedef wchar_t* (__cdecl* fn__wcsdup)(
    wchar_t const* _String
    );

typedef size_t(__cdecl* fn_wcslen)(
    wchar_t const* _String
    );

typedef BOOL(__stdcall* fn_D2Win_2766)(
    );

typedef BOOL(__fastcall* fn_D2Win_2715)(
    DWORD arg0,
    DWORD arg1,
    DWORD arg2,
    union program_setting_store* settings
    );

typedef int(__stdcall* fn_D2Win_2768)(
    );

typedef int(__stdcall* fn_D2Win_279e)(
    HMODULE module,
    DWORD videoMode,
    DWORD arg2,
    DWORD arg3
    );

typedef int(__stdcall* fn_D2Win_2744)(
    BOOL titleEnabled,
    BOOL isRes800
    );

typedef int(__stdcall* fn_D2Win_2730)(
    );

typedef int(__fastcall* fn_Fog_2725)(
    const char* windowClassName
    );

typedef int(__fastcall* fn_Fog_2775)(
    DWORD isDirect,
    DWORD arg1
    );

typedef BOOL(*fn_ErrorHandler)(
    );

typedef int(__fastcall* fn_Fog_2723)(
    const char* serverName,
    fn_ErrorHandler onError,
    const char* windowCaption,
    DWORD arg3
    );

typedef int(__fastcall* fn_Fog_2769)(
    DWORD arg0
    );

typedef int(__fastcall* fn_Fog_27ea)(
    );

typedef int(__fastcall* fn_Fog_276a)(
    );

typedef int(__cdecl* fn_Fog_279f)(
    DWORD arg0
    );

// 从 d2sound.dll 对应的代码来看，它将 ecx, edx 都保存到全局变量里。
// 所以应该是有两个参数的。
typedef int(__fastcall* fn_D2Sound_2712)(
    DWORD isExpansion,
    DWORD arg1
    );

typedef int(__fastcall* fn_D2Sound_272f)(
    );

typedef int(__stdcall* fn_D2Gfx_2764)(
    );

typedef void(__stdcall* fn_D2Gfx_2761)(
    DWORD arg0
    );

typedef void(__stdcall* fn_D2Gfx_2745)(
    );

typedef void(__stdcall* fn_D2Gfx_2732)(
    DWORD gamma
    );

typedef void(__stdcall* fn_D2Gfx_2717)(
    );

/*
* 获取游戏窗口句柄
*/
typedef HWND(__stdcall* fn_D2Gfx_2740)(
    );

typedef void(__cdecl* fn_Fog_27c9)(
    );

typedef void(__cdecl* fn_D2Common_294f)(
    );

typedef void(__cdecl* fn_D2Common_2950)(
    );

typedef void(__cdecl* fn_D2Game_2712)(
    );

typedef void(__cdecl* fn_D2Game_2713)(
    );

typedef void(__cdecl* fn_D2Game_2714)(
    );

typedef void(__cdecl* fn_D2Game_2715)(
    );

typedef void(__cdecl* fn_D2Game_2716)(
    );

typedef void(__cdecl* fn_D2Game_2717)(
    );

typedef void(__cdecl* fn_D2Game_2727)(
    );

typedef void(__cdecl* fn_D2Game_2728)(
    );

typedef void(__cdecl* fn_D2Game_273e)(
    );

typedef void(__cdecl* fn_D2Game_273f)(
    );

typedef void(__cdecl* fn_D2Game_2742)(
    );

typedef void(__cdecl* fn_D2Lang_2710)(
    );

typedef void(__cdecl* fn_D2Lang_2711)(
    );

typedef void(__cdecl* fn_D2Net_2713)(
    );

typedef void(__cdecl* fn_D2Net_2714)(
    );