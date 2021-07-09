#pragma once

#include <Windows.h>

/*
* 复制内存块数据到新申请的内存区域。
* @returns 返回复制出来的数据
*/
extern void* sub_40798d_DuplicateMemoryBlock(
    void* src,
    size_t size
);