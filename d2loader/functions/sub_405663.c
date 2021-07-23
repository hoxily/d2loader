#include "pch.h"
#include "sub_405663.h"
#include "../global-variables.h"
#include "sub_405763.h"

// 尝试加载指定的dll模块
// 注意跨行的宏定义，反斜杠后面不能有其他字符，只能是换行符。
#define TRY_LOAD_D2_MODULE(moduleHandle, moduleName) \
do \
{ \
    if (moduleHandle == NULL) \
    { \
        moduleHandle = LoadLibraryA(moduleName); \
    } \
    if (moduleHandle == NULL) \
    { \
        return FALSE; \
    } \
} while (FALSE); \

BOOL sub_405663_LoadDynamicFunctions(
)
{
    if (global_dd_4085d8_isDynamicFunctionsLoaded)
    {
        return TRUE;
    }

    TRY_LOAD_D2_MODULE(global_dd_4085b8_moduleFog, "fog.dll");
    TRY_LOAD_D2_MODULE(global_dd_4085c0_moduleD2Win, "d2win.dll");
    TRY_LOAD_D2_MODULE(global_dd_4085bc_moduleD2Sound, "d2sound.dll");
    TRY_LOAD_D2_MODULE(global_dd_4085c4_moduleD2Gfx, "d2gfx.dll");
    TRY_LOAD_D2_MODULE(global_dd_4085c8_moduleD2Game, "d2game.dll");
    TRY_LOAD_D2_MODULE(global_dd_4085cc_moduleD2Common, "d2common.dll");
    TRY_LOAD_D2_MODULE(global_dd_4085d0_moduleD2Net, "d2net.dll");
    TRY_LOAD_D2_MODULE(global_dd_4085d4_moduleD2Lang, "d2lang.dll");

    if (!sub_405763_InitD2FunctionTable())
    {
        return FALSE;
    }

    global_dd_4085d8_isDynamicFunctionsLoaded = TRUE;
    return TRUE;
}
