#pragma once

#include <Windows.h>
#include "../data-types.h"

BOOL sub_40513a(
    HMODULE hModule,
    const char* hookDll,
    struct hook_search_item* functionNameList,
    void* null1,
    void* null2
);
