#pragma once

#include "data-types.h"

/// <summary>
/// 这个函数比较特殊，使用eax寄存器来传递参数。
/// </summary>
void* sub_6fc07ef0(
    HD2ARCHIVE__* mpqHandle,
    const char* str,
    int arg2
);