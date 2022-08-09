// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "main.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        PluginMain(RUN_PLUGIN_REASON_INIT, nullptr);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        PluginMain(RUN_PLUGIN_REASON_CLEANUP, nullptr);
        break;
    }
    return TRUE;
}

