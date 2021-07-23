#include "pch.h"
#include "sub_405ebb.h"
#include <assert.h>
#include "../global-variables.h"
#include "sub_404ed0.h"

BOOL sub_405ebb_FreeCustomPatches(
)
{
    if (global_dd_4085e0_customPatches == NULL)
    {
        return TRUE;
    }

    struct patch_search_item* esi_ptr = global_dd_4085e0_customPatches;
    while (esi_ptr->moduleFileName != (void*)-1)
    {
        free(esi_ptr->moduleFileName);
        free(esi_ptr->v3);
        free(esi_ptr->v5);
        if (esi_ptr->pattern != NULL)
        {
            free(esi_ptr->pattern);
        }

        assert(sizeof(struct patch_search_item) == 0x2c);
        esi_ptr++;
    }
    free(global_dd_4085e0_customPatches);
    global_dd_4085e0_customPatches = NULL;

    return TRUE;
}