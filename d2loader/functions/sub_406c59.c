#include "pch.h"
#include <assert.h>
#include "sub_406c59.h"
#include "../global-variables.h"
#include "../constant-strings.h"
#include "sub_40735e.h"

void sub_406c59_CheckGameExeVersion(
)
{
    // GetFileVersionInfoSizeA 总是会把 unusedHandle 设为 0
    DWORD unusedHandle;
    UINT len;
    VS_FIXEDFILEINFO* fileInfo;

    DWORD edi_size = GetFileVersionInfoSizeA(CSTR_GAME_DOT_EXE, &unusedHandle);
    void* ebx_ptr = VirtualAlloc(NULL, edi_size, MEM_COMMIT, PAGE_READWRITE);
    UINT esi = 0;
    if (ebx_ptr) {
        // esi 已经在上面被 xor esi, esi 置为0；
        if (GetFileVersionInfoA(CSTR_GAME_DOT_EXE, 0, edi_size, ebx_ptr))
        {
            if (VerQueryValueA(ebx_ptr, "\\", &fileInfo, &len))
            {
                if (fileInfo)
                {
                    assert(offsetof(VS_FIXEDFILEINFO, dwFileVersionLS) == 0x0c);
                    // 检查 game.exe 的FileVersion是否为 1.0.13.60，
                    // 由于 dwFileVersionMS 必定为 0x00010000，所以只检查了 dwFileVersionLS
                    if (fileInfo->dwFileVersionLS != 0x000d003c)
                    {
                        MessageBoxA(NULL, CSTR_D2_LOADER_DOES_NOT_RECOGNIZE_GAME_VERSION, CSTR_UNKNOW_VERSION, MB_OK | MB_ICONERROR);
                        ExitProcess(1);
                    }

                    // VS_FIXEDFILEINFO 结构的大小是 0x34
                    assert(sizeof(VS_FIXEDFILEINFO) == 0x34);
                    if (len == sizeof(VS_FIXEDFILEINFO))
                    {
                        assert(offsetof(VS_FIXEDFILEINFO, dwProductVersionMS) == 0x10);
                        UINT version = fileInfo->dwProductVersionMS;
                        version >>= 16;
                        esi = version & 0xff;
                        version = fileInfo->dwProductVersionMS & 0xff;
                        esi <<= 8;
                        esi |= version;

                        assert(offsetof(VS_FIXEDFILEINFO, dwProductVersionLS) == 0x14);
                        version = fileInfo->dwProductVersionLS;
                        UINT eax = fileInfo->dwProductVersionLS & 0xff;
                        version >>= 16;
                        version = version & 0xff;
                        esi <<= 8;
                        esi |= version;
                        esi <<= 8;
                        esi |= eax;
                    }
                }
            }
        }
    }

    // 在清理之前还有两段汇编，但是只有出口通向清理，却没有入口。略过。

    // VirtualFree 要求第一个参数不为 NULL，所以用if包裹一下。
    if (ebx_ptr)
    {
        VirtualFree(ebx_ptr, 0, MEM_RELEASE);
    }

    if (sub_40735e_CheckExpansion())
    {
        // 最高比特位 置1
        esi |= 0x80000000;
    }
    else
    {
        // 最高比特位 置0
        esi &= 0x7fffffff;
    }
    global_dd_408620_settings->dd_07b4_gameProductVersionFlag.value = esi;
}
