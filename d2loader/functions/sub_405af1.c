#include "pch.h"
#include "sub_405af1.h"
#include "../global-variables.h"
#include "sub_405827.h"
#include "sub_4066d8.h"
#include "sub_405ebb.h"

BOOL sub_405af1_RestoreAllPatches(
)
{
    BOOL ret = sub_405827_PatchModule(
        global_dd_402e0c_builtinPatches,
        PATCH_ACTION_TYPE_RESTORE
    );
    if (!ret)
    {
        return FALSE;
    }

    sub_4066d8_AlwaysReturnTrue();

    if (global_dd_4085e0_customPatches != NULL)
    {
        sub_405827_PatchModule(
            global_dd_4085e0_customPatches,
            PATCH_ACTION_TYPE_RESTORE
        );
    }

    sub_405ebb_FreeCustomPatches();
    global_dd_4085dc_isPatchCompleted = FALSE;

    return TRUE;
}
