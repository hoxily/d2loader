#pragma once

#include "pch.h"

#define RUN_PLUGIN_REASON_INIT 0x1ul
#define RUN_PLUGIN_REASON_CLEANUP 0x2ul
#define RUN_PLUGIN_REASON_ENTER_GAME 0x4ul
#define RUN_PLUGIN_REASON_LEAVE_GAME 0x8ul
#define RUN_PLUGIN_REASON_ENTER_CHANNEL 0x10ul
#define RUN_PLUGIN_REASON_LEAVE_CHANNEL 0x20ul
#define RUN_PLUGIN_REASON_ENTER_MAIN_MENU 0x40ul
#define RUN_PLUGIN_REASON_LEAVE_MAIN_MENU 0x80ul
#define RUN_PLUGIN_REASON_ERROR_CLEANUP 0x100ul

extern "C" {

    typedef BOOL(__stdcall* fn_RunPlugin)(DWORD reasonFlag, DWORD* gameProductVersionFlag);

    BOOL __stdcall PluginMain(DWORD reasonFlag, DWORD* gameProductVersionFlag);

    struct query_interface_result
    {
        // 0x44320000
        DWORD magic;
        // 0x01000912
        DWORD version;
        const char* pluginName;
        fn_RunPlugin runPlugin;
    };

    struct query_interface_result* __stdcall QueryInterface();

    void __fastcall MyDrawHoverText(wchar_t* wStr, long xPos, long yPos, DWORD dwTran, DWORD dwColor);
    void __fastcall D2Win_DrawHoverText(wchar_t* wStr, long xPos, long yPos, DWORD dwTran, DWORD dwColor);
}