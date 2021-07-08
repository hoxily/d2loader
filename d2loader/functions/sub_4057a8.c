#include "sub_4057a8.h"
#include "../global-variables.h"
#include "sub_4066ad.h"
#include "sub_405827.h"
#include "sub_4066c3.h"
#include "sub_405d19.h"

BOOL sub_4057a8_Patch(
)
{
    if (global_dd_4085dc_isPatchCompleted)
    {
        return TRUE;
    }

    if (!sub_4066ad())
    {
        return FALSE;
    }

    if (!sub_405827_PatchModule(global_dd_402e0c, 0))
    {
        return FALSE;
    }

    if (!sub_4066c3())
    {
        return FALSE;
    }

    if (global_dd_408620_settings->db_0b90_scriptFile.value[0] == '\0')
    {
        sub_405d19(
            "d2hack.script",
            NULL
        );
    }
    else
    {
        sub_405d19(
            "d2hack.script",
            global_dd_408620_settings->db_0b90_scriptFile.value,
            NULL
        );
    }

    if (global_dd_4085e0_memory)
    {
        sub_405827_PatchModule(global_dd_4085e0_memory, 0);
    }

    return TRUE;
}
