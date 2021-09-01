#pragma once

#include "../D2LangApi.h"
#include "pch.h"

#ifdef __cplusplus
extern "C" {
#endif

    /// <summary>
    /// 计算指定数据块的散列值
    /// </summary>
    /// <param name="hashValue">指向输出散列值的指针</param>
    /// <param name="buffer">缓冲区</param>
    /// <param name="startIndex">起始下标索引（含）</param>
    /// <param name="endIndex">终止下标索引（不含）</param>
    D2_LANG_API void __stdcall d2lang_10001_GetDataHash(
        WORD* hashValue,
        BYTE* buffer,
        DWORD startIndex,
        DWORD endIndex
    );

#ifdef __cplusplus
}
#endif
