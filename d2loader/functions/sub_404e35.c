#include "pch.h"
#include "sub_404e35.h"
#include "../constants.h"
#include "sub_4061df.h"
#include "sub_404ed0.h"

void sub_404e35_CallPlugin(
    DWORD oldGameState,
    DWORD newGameState
)
{
    DWORD esi_reasonFlag = 0;
    // 这里不处理 GAME_STATE_SERVER
    switch (oldGameState)
    {
    case GAME_STATE_NONE:
        break;
    case GAME_STATE_CLIENT:
        esi_reasonFlag = RUN_PLUGIN_REASON_LEAVE_GAME;
        break;
    case GAME_STATE_MULTI:
        esi_reasonFlag = RUN_PLUGIN_REASON_LEAVE_CHANNEL;
        break;
    case GAME_STATE_LAUNCH:
        esi_reasonFlag = RUN_PLUGIN_REASON_LEAVE_MAIN_MENU;
        break;
    default:
        sub_404ed0_LogFormat(
            LOG_TAG,
            "Got Unknown PrevDll Entry %d",
            oldGameState
        );
        break;
    }

    // 这里不处理 GAME_STATE_SERVER
    switch (newGameState)
    {
    case GAME_STATE_NONE:
        break;
    case GAME_STATE_CLIENT:
        esi_reasonFlag |= RUN_PLUGIN_REASON_ENTER_GAME;
        break;
    case GAME_STATE_MULTI:
        esi_reasonFlag |= RUN_PLUGIN_REASON_ENTER_CHANNEL;
        break;
    case GAME_STATE_LAUNCH:
        esi_reasonFlag |= RUN_PLUGIN_REASON_ENTER_MAIN_MENU;
        break;
    default:
        sub_404ed0_LogFormat(
            LOG_TAG,
            "Got Unknow Dll Entry %d",
            // fix by hoxily@qq.com: 这里应该是log输出 newGameState，而不是 oldGameState。
            newGameState
        );
        break;
    }

    sub_4061df_PluginListRun(esi_reasonFlag);
}