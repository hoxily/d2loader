// dllmain.c : 定义 DLL 应用程序的入口点。
#include <Windows.h>
#include <stdio.h>

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

typedef void(__stdcall* fn_OnInitPlugin)(DWORD gameProductVersionFlag);

#pragma pack(1)
struct query_interface_result_old
{
    DWORD magic;
    const char* pluginName;
    fn_OnInitPlugin init;
};

void __stdcall OnInitPlugin(DWORD gameProductVersionFlag)
{
    FILE* logFile = fopen("OldFormatPluginDll.log", "at");
    if (logFile)
    {
        fprintf(logFile, "TestPlugin.OnInitPlugin: game product version: 0x%08x", gameProductVersionFlag);
        fprintf(logFile, "\n");
        fflush(logFile);
        fclose(logFile);
    }
}

struct query_interface_result_old qir = {
    0x44320000,
    "OldFormatPluginExample",
    OnInitPlugin
};

// 使用 __stdcall 之后函数名会被矫正，导致 GetProcAddress(edi_dll, "QueryInterface") 找不到函数。
// 需要使用导出声明，修正名字。
// 矫正后的名字可以通过IDA的Exports表，右键菜单取消勾选 Show demangled 来查看。

#pragma comment(linker, "/export:QueryInterface=_QueryInterface@0")
struct query_interface_result_old* __stdcall QueryInterface()
{
    return &qir;
}
