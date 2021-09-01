#pragma once

#include "../D2LangApi.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
* 求取字符串的Hash值，返回值范围为 [0, n - 1]。
* @param str 需要求Hash值的字符串
* @param n 限定返回值范围为 [0, n - 1]
* @returns 返回 [0, n - 1] 范围的Hash值。
*/
D2_LANG_API unsigned int __stdcall d2lang_10000_GetStringHash(
    const char* str,
    unsigned int n
);

#ifdef __cplusplus
}
#endif
