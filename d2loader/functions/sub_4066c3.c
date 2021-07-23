#include "pch.h"
#include "sub_4066c3.h"
#include "../global-variables.h"
#include "sub_4075c3.h"

BOOL sub_4066c3_PatchLocale(
)
{
    sub_4075c3_D2LocaleSetLocale(
        global_dd_408620_settings->db_07d4_locale.value
    );
    return TRUE;
}