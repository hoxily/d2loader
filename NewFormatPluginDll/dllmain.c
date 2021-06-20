// dllmain.c : 定义 DLL 应用程序的入口点。
#include "pch.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

struct query_interface_result
{
    // 0x44320000
    DWORD magic;
    // 0x01000912
    DWORD version;
    const char* pluginName;
};

struct query_interface_result qir = {
    0x44320000,
    0x01000912,
    "NewFormatPluginDllExample"
};

// 使用 __stdcall 之后函数名会被矫正，导致 GetProcAddress(edi_dll, "QueryInterface") 找不到函数。
// 需要使用导出声明，修正名字。
// 矫正后的名字可以通过IDA的Exports表，右键菜单取消勾选 Show demangled 来查看。

#pragma comment(linker, "/export:QueryInterface=_QueryInterface@0")
struct query_interface_result* __stdcall QueryInterface()
{
    return &qir;
}