#include "sub_4070d5.h"
#include "../global-variables.h"
#include "sub_4070aa.h"
#include "sub_404ed0.h"

BOOL sub_4070d5_D2CommonInit(
)
{
    global_dd_408624_moduleBnClient = sub_4070aa_D2LoadLibrary("bnclient.dll");
    if (global_dd_408624_moduleBnClient == NULL)
    {
        return FALSE;
    }
    //TODO
    return FALSE;
}