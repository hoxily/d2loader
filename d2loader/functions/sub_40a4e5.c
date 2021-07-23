#include "pch.h"
#include "sub_40a4e5.h"
#include "../global-variables.h"
#include "sub_40a380.h"
#include "sub_40aaf0.h"
#include "sub_40a740.h"
#include "sub_40a7ce.h"
#include "sub_40a6c3.h"

void sub_40a4e5_ProcessNoHideNoSleepAltColorArgs(
)
{
    if (global_db_40a551_isHideSleepAltColorArgsProcessed)
    {
        return;
    }

    sub_40a380();

    //TODO: 跳过 -altcolor 参数的处理。
    if (sub_40a4e5_SearchInCommandLine("-nohide") == TRUE)
    {
        sub_40a7ce_SetGameWindowNoHide();
    }

    if (sub_40a4e5_SearchInCommandLine("-nosleep") != TRUE)
    {
        sub_40a6c3();
    }

    sub_40aaf0();

    global_db_40a551_isHideSleepAltColorArgsProcessed = TRUE;
}
