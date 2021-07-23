#pragma once

#include "pch.h"

/*
* 使用给定的字符数组 separators 中的任一字符，切割 str 字符串。
* @param str 被切分的字符串
* @param separators 用以分隔的字符数组
* @param count 返回切割完成的字符串数组的元素个数。可以为NULL。
* @returns 切割后的字符串数组，以一个NULL结束。字符串指针数组以及指向的字符串都已经打包到单块malloc内存里。释放时只需free该指针即可。
*/
char** sub_407f21_SplitString(
    const char* str,
    const char* separators,
    DWORD* count
);