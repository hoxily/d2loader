#pragma once

#include <stdio.h>
#include "data-types.h"

extern const char* global_dd_4011b0_gameStateDlls[];

extern DWORD global_dd_404718;

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

extern struct patch_search_item global_dd_402d88[3];
struct patch_search_item* global_dd_402e0c;

DWORD global_dd_402e1c[2];
char global_db_402e24[];
DWORD global_dd_402e68[2];
char global_db_402e70[];

extern struct string_index_item global_dd_402ea8_CommandLineArgumentTable[93];

extern fn_LoadLibraryA global_dd_4085a0_LoadLibraryA;

extern fn_GetModuleFileNameA global_dd_4085a4_GetModuleFileNameA;

extern fn_FindWindowA global_dd_4085a8_FindWindowA;

extern void* global_dd_4085e0_memory;
extern int global_dd_4085e4_totalLoadedPatchCount;
extern int global_dd_4085e8;

extern struct hook_search_item global_dd_4023f0[3];

extern struct hook_search_item global_dd_402408[2];

extern HMODULE global_dd_408624_moduleBnClient;
extern HMODULE global_dd_408628_moduleD2Client;
extern HMODULE global_dd_40862c_moduleD2Launch;
extern HMODULE global_dd_408630_moduleD2Multi;

extern DWORD global_dd_408638_mpqFileHandle;

extern BOOL global_dd_4085d8_isDynamicFunctionsLoaded;
extern BOOL global_dd_4085dc_isPatchCompleted;
extern HMODULE global_dd_4085b8_moduleFog;
extern HMODULE global_dd_4085c0_moduleD2Win;
extern HMODULE global_dd_4085bc_moduleD2Sound;
extern HMODULE global_dd_4085c4_moduleD2Gfx;
extern HMODULE global_dd_4085c8_moduleD2Game;
extern HMODULE global_dd_4085cc_moduleD2Common;
extern HMODULE global_dd_4085d0_moduleD2Net;
extern HMODULE global_dd_4085d4_moduleD2Lang;

extern struct d2_dll_function_item global_dd_402500_functionTable[];
extern struct d2_dll_function_item global_dd_402570_functionTable[];
extern struct d2_dll_function_item* global_dd_402750_functionTable;

extern fn_D2Win_2766 global_dd_40867c;
extern fn_D2Win_2715 global_dd_4086cc;
extern fn_D2Win_2768 global_dd_40866c;
extern fn_D2Win_279e global_dd_40869c;
extern fn_D2Win_2744 global_dd_4086d4;
extern fn_D2Win_2730 global_dd_4086a0;
extern fn_Fog_2725 global_dd_408688;
extern fn_Fog_2775 global_dd_4086ec;
extern fn_Fog_2723 global_dd_4086c8;
extern fn_Fog_2769 global_dd_4086d0;
extern fn_Fog_27ea global_dd_4086c4;
extern fn_Fog_276a global_dd_408664;
extern fn_Fog_279f global_dd_4086a8;
extern fn_D2Sound_2712 global_dd_40868c;
extern fn_D2Sound_272f global_dd_4086e0;
extern fn_D2Gfx_2764 global_dd_408670;
extern fn_D2Gfx_2761 global_dd_4086c0;
extern fn_D2Gfx_2745 global_dd_408674;
extern fn_D2Gfx_2732 global_dd_4086b0;
extern fn_D2Gfx_2717 global_dd_4086e4;
extern fn_D2Gfx_2740 global_dd_4086ac;
extern fn_Fog_27c9 global_dd_4086dc;
extern fn_D2Common_294f global_dd_408658;
extern fn_D2Common_2950 global_dd_408694;
extern fn_D2Game_2712 global_dd_4086d8;
extern fn_D2Game_2713 global_dd_408660;
extern fn_D2Game_2714 global_dd_4086b8;
extern fn_D2Game_2715 global_dd_4086e8;
extern fn_D2Game_2716 global_dd_4086a4;
extern fn_D2Game_2717 global_dd_4086b4;
extern fn_D2Game_2727 global_dd_40865c;
extern fn_D2Game_2728 global_dd_408698;
extern fn_D2Game_273e global_dd_4086bc;
extern fn_D2Game_273f global_dd_408668;
extern fn_D2Game_2742 global_dd_4086f0;
extern fn_D2Lang_2710 global_dd_408684;
extern fn_D2Lang_2711 global_dd_408680;
extern fn_D2Net_2713 global_dd_408678;
extern fn_D2Net_2714 global_dd_408690;

extern HWND global_dd_408634_gameWindowHandle;

extern void* global_dd_40863c;
extern void* global_dd_408640;