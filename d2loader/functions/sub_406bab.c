#include "pch.h"
#include "sub_406bab.h"
#include "../global-variables.h"

BOOL sub_406bab_IsExpansion()
{
    if (global_dd_408620_settings == NULL)
    {
        return FALSE;
    }
    return global_dd_408620_settings->db_0000_expansion.value;
}
