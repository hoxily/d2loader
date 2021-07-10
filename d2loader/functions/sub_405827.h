#pragma once

#include <Windows.h>
#include "../data-types.h"

#define PATCH_ACTION_TYPE_APPLY 0
#define PATCH_ACTION_TYPE_RESTORE 1

extern BOOL sub_405827_PatchModule(
    struct patch_search_item* items,
    int actionType
);
