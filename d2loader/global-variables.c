#include <stdio.h>
#include "data-types.h"
#include "constants.h"
#include "global-variables.h"

/*
 * 数据类型前缀：
 * db: 1 字节
 * dw: 2 字节
 * dd: 4 字节
 */

// 全局变量的排列顺序最好能按照原始反汇编中的地址排列。这样容易发现重复定义的变量。
// 比如一个变量其实是一个数组，当取数组中的某一个元素时，在汇编代码中仍然是全局变量的形式，
// 当重复定义逻辑上相同的变量时，会引起数据错误。

const char* global_dd_4011b0_gameStateDlls[] =
{
    "none.dll",
    "d2client.dll",
    "d2server.dll",
    "d2multi.dll",
    "d2launch.dll"
};

DWORD global_dd_404718;

union program_setting_store* global_dd_408620_settings;
FILE* global_dd_408588_logFile;
FILE* global_dd_40858c_logFile;

DWORD global_dd_408590_logFlag;
char global_db_402958_pluginDir[0x104] = "Plugin";
DWORD global_dd_4085f0;
CRITICAL_SECTION global_dd_4085f8_criticalSection;

struct loaded_plugin_item* global_dd_408610_plugins;
DWORD global_dd_408614_count;
int global_dd_408618_activePluginCount;
DWORD global_dd_40861c_capacity;

const char* global_dd_408598_moduleD2LoaderFileName;
const char* global_dd_40859c_gameDotExeFileName;
HMODULE global_dd_4085b0_moduleBnClient;
HMODULE global_dd_4085ac_moduleD2Gfx;
HMODULE global_dd_4085b4_moduleD2Net;

struct patch_search_item global_dd_402d88[3] =
{
    {
        "d2lang.dll",
        0x3fd2, 4, &global_dd_402e1c[0], 0, NULL, 0,
        1, &global_dd_40863c, &global_db_402e24[0], 0
    },
    {
        "d2lang.dll",
        0x4010, 4, &global_dd_402e68[0], 0, NULL, 0,
        1, &global_dd_408640, &global_db_402e70[0], 0
    },
    {
        (const char*)(-1),
        0, 0, NULL, 0, NULL, 0,
        0, NULL, NULL, 0
    }
};

DWORD global_dd_402e1c[2] =
{
    0x910a20, 0
};

char global_db_402e24[] = "7, B8xxxxxxxx C605xxxxxxxx01 E8xxxxxxxx 0FB6086A00890D";

DWORD global_dd_402e68[2] =
{
    0x6fc10a1c, 0
};
char global_db_402e70[] = "2, 8B15xxxxxxxx 83FA0D 7C10 33D2 8915xxxxxxxx EB06 8B15";

struct patch_search_item* global_dd_402e0c = &global_dd_402d88[0];

struct string_index_item global_dd_402ea8_CommandLineArgumentTable[93] = {
    { "exp", "MAIN", "expansion", 0x0, ARG_TYPE_NO_PARAM },
    { "w", "VIDEO", "window", 0x4, ARG_TYPE_NO_PARAM },
    { "glide", "VIDEO", "glide", 0x6, ARG_TYPE_NO_PARAM },
    { "opengl", "VIDEO", "opengl", 0x7, ARG_TYPE_NO_PARAM },
    { "d3d", "VIDEO", "d3d", 0x9, ARG_TYPE_NO_PARAM },
    { "rave", "VIDEO", "rave", 0x8, ARG_TYPE_NO_PARAM },
    { "per", "VIDEO", "perspective", 0x9, ARG_TYPE_NO_PARAM },
    { "lq", "VIDEO", "lowquality", 0xa, ARG_TYPE_NO_PARAM },
    { "gamma", "VIDEO", "gamma", 0xc, ARG_TYPE_UNSIGNED_INT },
    { "vsync", "VIDEO", "vsync", 0x10, ARG_TYPE_NO_PARAM },
    { "fr", "VIDEO", "framerate", 0x10, ARG_TYPE_UNSIGNED_INT },
    { "s", "NETWORK", "serverip", 0x33, 0x18 },
    { "gametype", "NETWORK", "gametype", 0x14, ARG_TYPE_UNSIGNED_INT },
    { "joinid", "NETWORK", "joinid", 0x18, ARG_TYPE_UNSIGNED_SHORT_INT },
    { "gamename", "NETWORK", "gamename", 0x1a, 0x18 },
    { "bn", "NETWORK", "battlenetip", 0x4b, 0x18 },
    { "mcpip", "NETWORK", "mcpip", 0x63, 0x18 },
    { "nopk", "NETWORK", "nopk", 0x7e, ARG_TYPE_NO_PARAM },
    { "openc", "NETWORK", "openc", 0x7f, ARG_TYPE_NO_PARAM },
    { "arena", "GAME", "arena", 0x202, ARG_TYPE_UNSIGNED_SHORT_INT },
    { "difficulty", "GAME", "difficulty", 0x20b, ARG_TYPE_NO_PARAM },
    { "txt", "GAME", "text", 0x211, ARG_TYPE_NO_PARAM },
    // -ama 与 -pal 的偏移量是一样的。可能是 d2loader 的一个bug。
    { "ama", "CHARACTER", "ama", 0x81, ARG_TYPE_NO_PARAM },
    { "pal", "CHARACTER", "pal", 0x81, ARG_TYPE_NO_PARAM },
    { "sor", "CHARACTER", "sor", 0x82, ARG_TYPE_NO_PARAM },
    { "nec", "CHARACTER", "nec", 0x83, ARG_TYPE_NO_PARAM },
    { "bar", "CHARACTER", "bar", 0x84, ARG_TYPE_NO_PARAM },
    { "dru", "CHARACTER", "dru", 0x85, ARG_TYPE_NO_PARAM },
    { "asn", "CHARACTER", "asn", 0x86, ARG_TYPE_NO_PARAM },
    { "i", "CHARACTER", "invincible", 0x87, ARG_TYPE_NO_PARAM },
    { "bnacct", "CHARACTER", "bnacct", 0x88, 0x18 },
    { "bnpass", "CHARACTER", "bnpass", 0xa0, 0x18 },
    { "name", "CHARACTER", "name", 0xb9, 0x18 },
    { "realm", "CHARACTER", "realm", 0xd1, 0x18 },
    { "ctemp", "CHARACTER", "ctemp", 0x1e9, ARG_TYPE_NO_PARAM },
    { "charclass", "CHARACTER", "charclass", 0x1e9, ARG_TYPE_NO_PARAM },
    { "charstatus", "CHARACTER", "charstatus", 0x1ea, ARG_TYPE_UNSIGNED_SHORT_INT },
    { "nm", "MONSTER", "nomonster", 0x1ec, ARG_TYPE_NO_PARAM },
    { "m", "MONSTER", "monsterclass", 0x1ed, ARG_TYPE_UNSIGNED_INT },
    { "minfo", "MONSTER", "monsterinfo", 0x1f1, ARG_TYPE_NO_PARAM },
    { "md", "MONSTER", "monsterdebug", 0x1f2, ARG_TYPE_UNSIGNED_INT },
    { "rare", "ITEM", "rare", 0x1f6, ARG_TYPE_NO_PARAM },
    { "unique", "ITEM", "unique", 0x1f7, ARG_TYPE_NO_PARAM },
    { "act", "INTERFACE", "act", 0x1fb, ARG_TYPE_UNSIGNED_INT },
    { "log", "DEBUGS", "log", 0x212, ARG_TYPE_NO_PARAM },
    { "msglog", "DEBUGS", "msglog", 0x212, ARG_TYPE_NO_PARAM },
    { "safe", "DEBUGS", "safemode", 0x213, ARG_TYPE_NO_PARAM },
    { "nosave", "DEBUGS", "nosave", 0x215, ARG_TYPE_NO_PARAM },
    { "seed", "DEBUGS", "seed", 0x216, ARG_TYPE_UNSIGNED_INT },
    { "teen", "DEBUGS", "teen", 0x21a, ARG_TYPE_NO_PARAM },
    { "cheats", "DEBUGS", "cheats", 0x219, ARG_TYPE_NO_PARAM },
    { "ns", "DEBUGS", "nosound", 0x21b, ARG_TYPE_NO_PARAM },
    { "questall", "DEBUGS", "questall", 0x21c, ARG_TYPE_NO_PARAM },
    { "build", "DEBUGS", "build", 0x21e, ARG_TYPE_NO_PARAM },
    { "npl", "FILEIO", "nopreload", 0x1fe, ARG_TYPE_NO_PARAM },
    { "direct", "FILEIO", "direct", 0x1ff, ARG_TYPE_NO_PARAM },
    { "lem", "FILEIO", "lowend", 0x200, ARG_TYPE_NO_PARAM },
    { "nocompress", "FILEIO", "nocompress", 0x202, ARG_TYPE_NO_PARAM },
    { "comint", "BNET", "comint", 0x221, ARG_TYPE_UNSIGNED_INT },
    { "token", "BNET", "token", 0x223, ARG_TYPE_UNSIGNED_INT },
    { "gamepass", "BNET", "gamepass", 0x23f, 0x18 },
    { "skiptobnet", "BNET", "skiptobnet", 0x359, ARG_TYPE_NO_PARAM },
    { "client", "CUSTOM", "client", 0x79b, ARG_TYPE_NO_PARAM },
    { "server", "CUSTOM", "server", 0x79c, ARG_TYPE_NO_PARAM },
    { "launch", "CUSTOM", "launch", 0x79d, ARG_TYPE_NO_PARAM },
    { "multi", "CUSTOM", "multi", 0x79e, ARG_TYPE_NO_PARAM },
    { "notitle", "CUSTOM", "notitle", 0x79f, ARG_TYPE_NO_PARAM },
    { "res800", "CUSTOM", "res800", 0x7a0, ARG_TYPE_NO_PARAM },
    { "res640", "CUSTOM", "res640", 0x7a1, ARG_TYPE_NO_PARAM },
    { "nonotify", "CUSTOM", "nonotify", 0x7a2, ARG_TYPE_NO_PARAM },
    { "noexit", "CUSTOM", "noexit", 0x7a3, ARG_TYPE_NO_PARAM },
    { "autorest", "CUSTOM", "autorestart", 0x7a4, ARG_TYPE_NO_PARAM },
    { "nohook", "CUSTOM", "nohook", 0x7a5, ARG_TYPE_NO_PARAM },
    { "nochar", "CUSTOM", "nochar", 0x7a6, ARG_TYPE_NO_PARAM },
    { "clientexit", "CUSTOM", "clientexit", 0x7a7, ARG_TYPE_NO_PARAM },
    { "multiclient", "CUSTOM", "multiclient", 0x7a8, ARG_TYPE_NO_PARAM },
    { "printarg", "CUSTOM", "printarg", 0x7a9, ARG_TYPE_NO_PARAM },
    { "noscript", "CUSTOM", "noscript", 0x7aa, ARG_TYPE_NO_PARAM },
    { "noplugin", "CUSTOM", "noplugin", 0x7ab, ARG_TYPE_NO_PARAM },
    { "l", "CUSTOM", "debuglog", 0x7ac, ARG_TYPE_NO_PARAM },
    { "console", "CUSTOM", "console", 0x7ad, ARG_TYPE_NO_PARAM },
    { "rmconffile", "CUSTOM", "rmconffile", 0x7ae, ARG_TYPE_NO_PARAM },
    { "nocleanup", "CUSTOM", "nocleanup", 0x7af, ARG_TYPE_NO_PARAM },
    { "servername", "CUSTOM", "servername", 0x7ec, 0x18 },
    { "pri", "CUSTOM", "priority", 0x7bc, 0x18 },
    { "title", "CUSTOM", "title", 0x804, 0x80 },
    { "locale", "CUSTOM", "locale", 0x7d4, 0x18 },
    { "cfg", "CUSTOM", "conffile", 0x884, 0x104 },
    { "mpq", "CUSTOM", "mpqfile", 0x988, 0x104 },
    { "pdir", "CUSTOM", "plugindir", 0xa8c, 0x104 },
    { "sfile", "CUSTOM", "scriptfile", 0xb90, 0x104 },
    { "test", "CUSTOM", "test", 0x7b8, ARG_TYPE_UNSIGNED_INT },
    { NULL, NULL, NULL, 0, 0 },
};

fn_LoadLibraryA global_dd_4085a0_LoadLibraryA;

fn_GetModuleFileNameA global_dd_4085a4_GetModuleFileNameA;

fn_FindWindowA global_dd_4085a8_FindWindowA;

void* global_dd_4085e0_memory = NULL;
int global_dd_4085e4_totalLoadedPatchCount = 0;
int global_dd_4085e8 = 0;

#include "functions/sub_4054c2.h"
#include "functions/sub_40543b.h"

struct hook_search_item global_dd_4023f0[3] =
{
    { "LoadLibraryA", sub_4054c2_my_LoadLibraryA },
    { "GetModuleFileNameA", sub_40543b_my_GetModuleFileNameA },
    { (const char*)-1, NULL }
};

#include "functions/sub_4053fd.h"

struct hook_search_item global_dd_402408[2] =
{
    { "FindWindowA", sub_4053fd_my_FindWindowA },
    { (const char*)-1, NULL }
};

HMODULE global_dd_408624_moduleBnClient;
HMODULE global_dd_408628_moduleD2Client;
HMODULE global_dd_40862c_moduleD2Launch;
HMODULE global_dd_408630_moduleD2Multi;

DWORD global_dd_408638_mpqFileHandle;

BOOL global_dd_4085d8_isDynamicFunctionsLoaded = FALSE;
BOOL global_dd_4085dc_isPatchCompleted = FALSE;

HMODULE global_dd_4085b8_moduleFog = NULL;
HMODULE global_dd_4085c0_moduleD2Win = NULL;
HMODULE global_dd_4085bc_moduleD2Sound = NULL;
HMODULE global_dd_4085c4_moduleD2Gfx = NULL;
HMODULE global_dd_4085c8_moduleD2Game = NULL;
HMODULE global_dd_4085cc_moduleD2Common = NULL;
HMODULE global_dd_4085d0_moduleD2Net = NULL;
HMODULE global_dd_4085d4_moduleD2Lang = NULL;

struct d2_dll_function_item global_dd_402500_functionTable[] =
{
    { &global_dd_4085c0_moduleD2Win, (void**)&global_dd_40867c, 0x2731 },
    //TODO
};

struct d2_dll_function_item global_dd_402570_functionTable[] =
{
    { &global_dd_4085c0_moduleD2Win, (void**)&global_dd_40867c, 0x2766 },
    { &global_dd_4085c0_moduleD2Win, (void**)&global_dd_4086cc, 0x2715 },
    { &global_dd_4085c0_moduleD2Win, (void**)&global_dd_40866c, 0x2768 },
    { &global_dd_4085c0_moduleD2Win, (void**)&global_dd_40869c, 0x279e },
    { &global_dd_4085c0_moduleD2Win, (void**)&global_dd_4086d4, 0x2744 },
    { &global_dd_4085c0_moduleD2Win, (void**)&global_dd_4086a0, 0x2730 },
    { &global_dd_4085b8_moduleFog, (void**)&global_dd_408688, 0x2725 },
    { &global_dd_4085b8_moduleFog, (void**)&global_dd_4086ec, 0x2775 },
    { &global_dd_4085b8_moduleFog, (void**)&global_dd_4086c8, 0x2723 },
    { &global_dd_4085b8_moduleFog, (void**)&global_dd_4086d0, 0x2769 },
    { &global_dd_4085b8_moduleFog, (void**)&global_dd_4086c4, 0x27ea },
    { &global_dd_4085b8_moduleFog, (void**)&global_dd_408664, 0x276a },
    { &global_dd_4085b8_moduleFog, (void**)&global_dd_4086a8, 0x279f },
    { &global_dd_4085bc_moduleD2Sound, (void**)&global_dd_40868c, 0x2712 },
    { &global_dd_4085bc_moduleD2Sound, (void**)&global_dd_4086e0, 0x272f },
    { &global_dd_4085c4_moduleD2Gfx, (void**)&global_dd_408670, 0x2764 },
    { &global_dd_4085c4_moduleD2Gfx, (void**)&global_dd_4086c0, 0x2761 },
    { &global_dd_4085c4_moduleD2Gfx, (void**)&global_dd_408674, 0x2745 },
    { &global_dd_4085c4_moduleD2Gfx, (void**)&global_dd_4086b0, 0x2732 },
    { &global_dd_4085c4_moduleD2Gfx, (void**)&global_dd_4086e4, 0x2717 },
    { &global_dd_4085c4_moduleD2Gfx, (void**)&global_dd_4086ac, 0x2740 },
    { &global_dd_4085b8_moduleFog, (void**)&global_dd_4086dc, 0x27c9 },
    { &global_dd_4085cc_moduleD2Common, (void**)&global_dd_408658, 0x294f },
    { &global_dd_4085cc_moduleD2Common, (void**)&global_dd_408694, 0x2950 },
    { &global_dd_4085c8_moduleD2Game, (void**)&global_dd_4086d8, 0x2712 },
    { &global_dd_4085c8_moduleD2Game, (void**)&global_dd_408660, 0x2713 },
    { &global_dd_4085c8_moduleD2Game, (void**)&global_dd_4086b8, 0x2714 },
    { &global_dd_4085c8_moduleD2Game, (void**)&global_dd_4086e8, 0x2715 },
    { &global_dd_4085c8_moduleD2Game, (void**)&global_dd_4086a4, 0x2716 },
    { &global_dd_4085c8_moduleD2Game, (void**)&global_dd_4086b4, 0x2717 },
    { &global_dd_4085c8_moduleD2Game, (void**)&global_dd_40865c, 0x2727 },
    { &global_dd_4085c8_moduleD2Game, (void**)&global_dd_408698, 0x2728 },
    { &global_dd_4085c8_moduleD2Game, (void**)&global_dd_4086bc, 0x273e },
    { &global_dd_4085c8_moduleD2Game, (void**)&global_dd_408668, 0x273f },
    { &global_dd_4085c8_moduleD2Game, (void**)&global_dd_4086f0, 0x2742 },
    { &global_dd_4085d4_moduleD2Lang, (void**)&global_dd_408684, 0x2710 },
    { &global_dd_4085d4_moduleD2Lang, (void**)&global_dd_408680, 0x2711 },
    { &global_dd_4085d0_moduleD2Net, (void**)&global_dd_408678, 0x2713 },
    { &global_dd_4085d0_moduleD2Net, (void**)&global_dd_408690, 0x2714 },
    { NULL, NULL, 0 }
};

struct d2_dll_function_item* global_dd_402750_functionTable = &global_dd_402570_functionTable[0];

fn_D2Win_2766 global_dd_40867c;
fn_D2Win_2715 global_dd_4086cc;
fn_D2Win_2768 global_dd_40866c;
fn_D2Win_279e global_dd_40869c;
fn_D2Win_2744 global_dd_4086d4;
fn_D2Win_2730 global_dd_4086a0;
fn_Fog_2725 global_dd_408688;
fn_Fog_2775 global_dd_4086ec;
fn_Fog_2723 global_dd_4086c8;
fn_Fog_2769 global_dd_4086d0;
fn_Fog_27ea global_dd_4086c4;
fn_Fog_276a global_dd_408664;
fn_Fog_279f global_dd_4086a8;
fn_D2Sound_2712 global_dd_40868c;
fn_D2Sound_272f global_dd_4086e0;
fn_D2Gfx_2764 global_dd_408670;
fn_D2Gfx_2761 global_dd_4086c0;
fn_D2Gfx_2745 global_dd_408674;
fn_D2Gfx_2732 global_dd_4086b0;
fn_D2Gfx_2717 global_dd_4086e4;
fn_D2Gfx_2740 global_dd_4086ac;
fn_Fog_27c9 global_dd_4086dc;
fn_D2Common_294f global_dd_408658;
fn_D2Common_2950 global_dd_408694;
fn_D2Game_2712 global_dd_4086d8;
fn_D2Game_2713 global_dd_408660;
fn_D2Game_2714 global_dd_4086b8;
fn_D2Game_2715 global_dd_4086e8;
fn_D2Game_2716 global_dd_4086a4;
fn_D2Game_2717 global_dd_4086b4;
fn_D2Game_2727 global_dd_40865c;
fn_D2Game_2728 global_dd_408698;
fn_D2Game_273e global_dd_4086bc;
fn_D2Game_273f global_dd_408668;
fn_D2Game_2742 global_dd_4086f0;
fn_D2Lang_2710 global_dd_408684;
fn_D2Lang_2711 global_dd_408680;
fn_D2Net_2713 global_dd_408678;
fn_D2Net_2714 global_dd_408690;

HWND global_dd_408634_gameWindowHandle;

void* global_dd_40863c;
void* global_dd_408640;