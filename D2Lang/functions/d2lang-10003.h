#pragma once

#include "../D2LangApi.h"
#include "pch.h"

#ifdef __cplusplus
extern "C" {
#endif

    /// <summary>
    /// 根据字符串ID获取本地化的字符串
    /// </summary>
    /// <param name="stringId">字符串ID</param>
    /// <returns>本地化的字符串</returns>
    D2_LANG_API wchar_t* __fastcall d2lang_10003_GetLocalText(
        WORD stringId
    );

#ifdef __cplusplus
}
#endif
