#pragma once

#include "data-types-forward-structs.h"

// fn_OnInitPlugin ������ջƽ���Ǻ����ڲ����ġ�
typedef void(__stdcall* fn_OnInitPlugin)(DWORD gameProductVersionFlag);

// fn_RunPlugin ������ջƽ���Ǻ����ڲ����ġ�
typedef BOOL(__stdcall* fn_RunPlugin)(DWORD reasonFlag, DWORD* gameProductVersionFlag);

// ���ڲ������������� __stdcall �� __cdecl һ�¡�
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

typedef BOOL(__fastcall* fn_D2Win_2766)(
    );

typedef BOOL(__fastcall* fn_D2Win_2715)(
    DWORD arg0,
    DWORD arg1,
    DWORD arg2,
    union program_setting_store* settings
    );