#include "sub_404d77.h"
#include "../global-variables.h"
#include "../constants.h"
#include "sub_404ed0.h"
#include "sub_404e35.h"
#include "sub_406108.h"
#include "sub_407246.h"
#include "sub_4073ac.h"

BOOL sub_404d77_GameClientLoop(
)
{
    DWORD esi_gameState = global_dd_408620_settings->dd_07b0_gameState.value;
    DWORD edi_oldGameState = GAME_STATE_NONE;
    do
    {
        if (esi_gameState == GAME_STATE_NONE || esi_gameState >= GAME_STATE_INVALID)
        {
            return TRUE;
        }

        sub_404ed0_LogFormat(
            LOG_TAG,
            "Current Dll Entry is %s",
            global_dd_4011b0_gameStateDlls[esi_gameState]
        );

        sub_404e35(edi_oldGameState, esi_gameState);
        sub_406108();

        edi_oldGameState = global_dd_408620_settings->dd_07b0_gameState.value;
        esi_gameState = sub_407246(edi_oldGameState);
        if (esi_gameState == GAME_STATE_CLIENT
            && global_dd_408620_settings->db_07a8_multiClient.value
            && global_dd_408620_settings->dd_0014_gameType.value == 3)
        {
            sub_404ed0_LogFormat(
                LOG_TAG,
                "Trying to Run MultiClients"
            );
            sub_4073ac();

            esi_gameState = GAME_STATE_LAUNCH;
        }

        global_dd_408620_settings->dd_07b0_gameState.value = esi_gameState;
    } while (!global_dd_408620_settings->db_07a7_clientExit.value);

    sub_404ed0_LogFormat(
        LOG_TAG,
        "Break From MultiClient Child Window"
    );
    return TRUE;
}
