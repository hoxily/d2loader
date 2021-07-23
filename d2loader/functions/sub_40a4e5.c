#include "pch.h"
#include "sub_40a4e5.h"
#include "../global-variables.h"
#include "sub_40a380.h"
#include "sub_40aaf0.h"
#include "sub_40a740.h"
#include "sub_40a7ce.h"
#include "sub_40a6c3.h"
#include "sub_40a91f.h"

void sub_40a4e5_ProcessNoHideNoSleepAltColorArgs(
)
{
    if (global_db_40a551_isHideSleepAltColorArgsProcessed)
    {
        return;
    }

    sub_40a380();

    if (sub_40a4e5_SearchInCommandLine("-nohide") == TRUE)
    {
        sub_40a7ce_SetGameWindowNoHide();
    }

    if (sub_40a4e5_SearchInCommandLine("-nosleep") != TRUE)
    {
        sub_40a6c3();
    }

    if (sub_40a4e5_SearchInCommandLine("-altcolor") == TRUE)
    {
        sub_40a91f();
    }

    sub_40aaf0();

    global_db_40a551_isHideSleepAltColorArgsProcessed = TRUE;
}
