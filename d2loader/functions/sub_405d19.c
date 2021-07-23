#include "pch.h"
#include "sub_405d19.h"
#include <assert.h>
#include <stdarg.h>
#include "../global-variables.h"
#include "sub_405d90.h"
#include "sub_405ebb.h"
#include "sub_404ed0.h"

void sub_405d19_ScriptLoad(
    const char* d2HackScriptFileName,
    ...
)
{
    va_list ap;

    if (global_dd_4085e0_customPatches != NULL)
    {
        sub_405ebb_FreeCustomPatches();
    }

    global_dd_4085e4_totalLoadedPatchCount = 0;
    global_dd_4085e8 = 0;

    sub_405d90_ScriptLoadFile(d2HackScriptFileName);

    va_start(ap, d2HackScriptFileName);
    const char* fileName;
    while ((fileName = va_arg(ap, const char*)) != NULL)
    {
        sub_405d90_ScriptLoadFile(fileName);
    }
    va_end(ap);

    if (global_dd_4085e0_customPatches != NULL)
    {
        global_dd_4085e0_customPatches[
            global_dd_4085e4_totalLoadedPatchCount
        ].moduleFileName = (void*)-1;
    }

    sub_404ed0_LogFormat(
        LOG_TAG,
        "Total %d Patches Loaded",
        global_dd_4085e4_totalLoadedPatchCount
    );
}