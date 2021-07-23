#include "pch.h"
#include <assert.h>
#include "sub_4054fd.h"
#include "../global-variables.h"
#include "sub_40513a.h"

BOOL sub_4054fd_HookDll(
)
{
    HMODULE hModuleKernel32 = GetModuleHandleA("Kernel32.dll");
    assert(hModuleKernel32 != NULL);
    global_dd_4085a0_LoadLibraryA = (fn_LoadLibraryA)GetProcAddress(hModuleKernel32, "LoadLibraryA");
    global_dd_4085a4_GetModuleFileNameA = (fn_GetModuleFileNameA)GetProcAddress(hModuleKernel32, "GetModuleFileNameA");

    HMODULE hModuleUser32 = GetModuleHandleA("User32.dll");
    assert(hModuleUser32 != NULL);
    global_dd_4085a8_FindWindowA = (fn_FindWindowA)GetProcAddress(hModuleUser32, "FindWindowA");

    char filename[0x104];
    GetModuleFileNameA(NULL, filename, sizeof(filename));
    global_dd_408598_moduleD2LoaderFileName = _strdup(filename);

    char* filePart;
    GetFullPathNameA("game.exe", sizeof(filename), filename, &filePart);
    global_dd_40859c_gameDotExeFileName = _strdup(filename);

    /*
    * 这儿有段代码比较难理解。
    * mov [esp + 118h + lpLibFileName], offset LibFileName ; "bnclient.dll"
    *
    * 其中的 lpLibFileName 其实就是 -118h，相当于把当前栈顶元素赋予这个字符串常量首地址。
    *
    * 最神奇的是在这之前调用了 __cdecl 规范的 _strdup 函数，还没有平衡栈。
    * 也就相当于 pop ecx; mov ecx, offset LibFileName; push ecx;
    */
    global_dd_4085b0_moduleBnClient = LoadLibraryA("bnclient.dll");
    global_dd_4085ac_moduleD2Gfx = LoadLibraryA("d2gfx.dll");
    global_dd_4085b4_moduleD2Net = LoadLibraryA("d2net.dll");

    BOOL ret1 = sub_40513a(
        global_dd_4085b0_moduleBnClient,
        "Kernel32.dll",
        &global_dd_4023f0[0],
        NULL,
        NULL
    );
    BOOL ret2 = sub_40513a(
        global_dd_4085ac_moduleD2Gfx,
        "User32.dll",
        &global_dd_402408[0],
        NULL,
        NULL
    );

    return ret1 && ret2;
}
