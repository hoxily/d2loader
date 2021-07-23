#include "pch.h"
#include "sub_4071a5.h"
#include "../global-variables.h"
#include "sub_40579c.h"
#include "../storm-api.h"

BOOL sub_4071a5(
)
{
    BOOL ret;
    if (!global_dd_408620_settings->db_021b_noSound.value)
    {
        ret = global_dd_4086e0();
    }

    ret = global_dd_4086a0();
    ret = global_dd_408670();
    ret = global_dd_40866c();
    ret = global_dd_408664();
    ret = global_dd_4086a8(0);

    sub_40579c_AlwaysReturnTrue();

    if (global_dd_408630_moduleD2Multi != NULL)
    {
        // fix by hoxily@qq.com: 这里的NULL检查应该是针对 global_dd_408630_moduleD2Multi
        // 因为下方又释放了一遍 global_dd_40862c_moduleD2Launch
        FreeLibrary(global_dd_408630_moduleD2Multi);
    }

    if (global_dd_408624_moduleBnClient != NULL)
    {
        FreeLibrary(global_dd_408624_moduleBnClient);
    }

    if (global_dd_408628_moduleD2Client != NULL)
    {
        FreeLibrary(global_dd_408628_moduleD2Client);
    }

    if (global_dd_40862c_moduleD2Launch != NULL)
    {
        FreeLibrary(global_dd_40862c_moduleD2Launch);
    }

    if (global_dd_408638_mpqFileHandle != 0)
    {
        dll_storm_252(global_dd_408638_mpqFileHandle);
    }

    return TRUE;
}
