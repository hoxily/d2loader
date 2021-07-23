#include "pch.h"
#include "sub_40a4e5.h"
#include "../global-variables.h"
#include "sub_40a380.h"
#include "sub_40aaf0.h"

void sub_40a4e5_ProcessNoHideNoSleepAltColorArgs(
)
{
    if (global_db_40a551_isHideSleepAltColorArgsProcessed)
    {
        return;
    }

    sub_40a380();

    //TODO: 跳过 -nohide, -nosleep, -altcolor 参数的处理。

    sub_40aaf0();

    global_db_40a551_isHideSleepAltColorArgsProcessed = TRUE;
}
