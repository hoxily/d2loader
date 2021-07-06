#pragma once

#include <stdio.h>
#include "data-types.h"

extern union program_setting_store* global_dd_408620_settings;
extern FILE* global_dd_408588_logFile;
extern FILE* global_dd_40858c_logFile;

extern DWORD global_dd_408590_logFlag;
extern char global_db_402958_pluginDir[0x104];
extern DWORD global_dd_4085f0;
extern CRITICAL_SECTION global_dd_4085f8_criticalSection;

extern struct loaded_plugin_item* global_dd_408610_plugins;
extern DWORD global_dd_408614_count;
extern int global_dd_408618_activePluginCount;
extern DWORD global_dd_40861c_capacity;

extern const char* global_dd_408598_moduleD2LoaderFileName;
extern const char* global_dd_40859c_gameDotExeFileName;
extern HMODULE global_dd_4085b0_moduleBnClient;
extern HMODULE global_dd_4085ac_moduleD2Gfx;
extern HMODULE global_dd_4085b4_moduleD2Net;

extern struct string_index_item global_dd_402ea8_CommandLineArgumentTable[93];

extern fn_LoadLibraryA global_dd_4085a0_LoadLibraryA;

extern fn_GetModuleFileNameA global_dd_4085a4_GetModuleFileNameA;

extern fn_FindWindowA global_dd_4085a8_FindWindowA;

extern struct hook_search_item global_dd_4023f0[3];

extern struct hook_search_item global_dd_402408[2];

extern HMODULE global_dd_408624_moduleBnClient;
extern HMODULE global_dd_408628_moduleD2Client;
extern HMODULE global_dd_40862c_moduleD2Launch;

extern DWORD global_dd_408638_mpqFileHandle;

extern BOOL global_dd_4085d8_isOtherDllLoaded;
extern HMODULE global_dd_4085b8_moduleFog;
extern HMODULE global_dd_4085c0_moduleD2Win;
extern HMODULE global_dd_4085bc_moduleD2Sound;
extern HMODULE global_dd_4085c4_moduleD2Gfx;
extern HMODULE global_dd_4085c8_moduleD2Game;
extern HMODULE global_dd_4085cc_moduleD2Common;
extern HMODULE global_dd_4085d0_moduleD2Net;
extern HMODULE global_dd_4085d4_moduleD2Lang;

extern void* global_dd_402750;