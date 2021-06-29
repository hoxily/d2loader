#include "sub_406887.h"
#include "../global-variables.h"
#include "../constants.h"
#include "sub_406a68.h"
#include "sub_406ac0.h"
#include "sub_406b12.h"

BOOL sub_406887_ProcessCommandLineArguments(
    const char* commandLine
)
{
    char buffer[0x100];
    const char* edi_s = commandLine;
    do
    {
        edi_s = sub_406a68_CutFirstArgument(edi_s, buffer);
        int i = sub_406ac0_FindArgumentNameIndex(buffer);
        // test esi, esi
        // jl
        // 这样的组合表示检测esi是否为负数。
        if (i >= 0)
        {
            if (global_dd_402ea8_CommandLineArgumentTable[i].type != ARG_TYPE_NO_PARAM)
            {
                edi_s = sub_406a68_CutFirstArgument(edi_s, buffer);
            }

            sub_406b12_StoreCommandLineSetting(i, buffer);
        }
    } while (*edi_s != '\0');

    return TRUE;
}
