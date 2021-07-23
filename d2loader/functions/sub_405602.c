#include "pch.h"
#include "sub_405602.h"
#include "../global-variables.h"

#define TRY_FREE_MODULE(hModule) \
do\
{\
    if (hModule != NULL)\
    {\
        FreeLibrary(hModule);\
    }\
} while (0)\

BOOL sub_405602(
)
{
    TRY_FREE_MODULE(global_dd_4085b4_moduleD2Net);
    TRY_FREE_MODULE(global_dd_4085ac_moduleD2Gfx);
    TRY_FREE_MODULE(global_dd_4085b0_moduleBnClient);

    return TRUE;
}
