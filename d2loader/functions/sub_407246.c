#include "sub_407246.h"
#include "../global-variables.h"
#include "../constants.h"
#include "sub_404ed0.h"

DWORD sub_407246_D2DllEntry(
    DWORD targetGameState
)
{
    if (targetGameState >= GAME_STATE_INVALID)
    {
        sub_404ed0_LogFormat(
            LOG_TAG,
            "Got Bad Dll Entry %d",
            targetGameState
        );
        return GAME_STATE_NONE;
    }

    const char* fileName = global_dd_4011b0_gameStateDlls[targetGameState];

    HMODULE hModule = LoadLibraryA(
        fileName
    );
    if (hModule == NULL)
    {
        sub_404ed0_LogFormat(
            LOG_TAG,
            "Error Loading %s",
            fileName
        );
        return GAME_STATE_NONE;
    }

    fn_D2_QueryInterface query = (fn_D2_QueryInterface)GetProcAddress(
        hModule,
        "QueryInterface"
    );
    if (query == NULL)
    {
        sub_404ed0_LogFormat(
            LOG_TAG,
            "Can not QueryInterface from %s",
            fileName
        );
        return GAME_STATE_NONE;
    }

    fn_D2_ComInt fn = *(fn_D2_ComInt*)query();
    return (DWORD)fn(global_dd_408620_settings);
}