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