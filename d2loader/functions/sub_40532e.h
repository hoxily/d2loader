#pragma once

#include <Windows.h>

extern IMAGE_IMPORT_DESCRIPTOR* sub_40532e(
    HMODULE hModule,
    const char* hookDll
);
