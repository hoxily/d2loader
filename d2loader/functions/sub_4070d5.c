#include "sub_4070d5.h"
#include "../global-variables.h"
#include "sub_4070aa.h"
#include "sub_404ed0.h"
#include "sub_405663.h"

BOOL sub_4070d5_D2CommonInit(
)
{
    global_dd_408624_moduleBnClient = sub_4070aa_D2LoadLibrary("bnclient.dll");
    if (global_dd_408624_moduleBnClient == NULL)
    {
        return FALSE;
    }

    global_dd_408628_moduleD2Client = sub_4070aa_D2LoadLibrary("d2client.dll");
    if (global_dd_408628_moduleD2Client == NULL)
    {
        return FALSE;
    }

    global_dd_40862c_moduleD2Launch = sub_4070aa_D2LoadLibrary("d2launch.dll");
    if (global_dd_40862c_moduleD2Launch == NULL)
    {
        return FALSE;
    }

    if (sub_405663_LoadOtherDll())
    {

    }
    else
    {

    }
    //TODO
    return FALSE;
}