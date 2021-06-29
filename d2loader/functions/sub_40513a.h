#pragma once

#include <Windows.h>
#include "../data-types.h"

extern BOOL sub_40513a(
    HMODULE hModule,
    const char* hookDll,
    struct hook_search_item* functionNameList,
    void* null1,
    void* null2
);
