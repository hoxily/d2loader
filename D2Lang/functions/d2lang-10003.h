#pragma once

#include "../D2LangApi.h"
#include "pch.h"

#ifdef __cplusplus
extern "C" {
#endif

    /// <summary>
    /// �����ַ���ID��ȡ���ػ����ַ���
    /// </summary>
    /// <param name="stringId">�ַ���ID</param>
    /// <returns>���ػ����ַ���</returns>
    D2_LANG_API wchar_t* __fastcall d2lang_10003_GetLocalText(
        WORD stringId
    );

#ifdef __cplusplus
}
#endif
