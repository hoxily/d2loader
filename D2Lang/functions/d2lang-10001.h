#pragma once

#include "../D2LangApi.h"
#include "pch.h"

#ifdef __cplusplus
extern "C" {
#endif

    /// <summary>
    /// ����ָ�����ݿ��ɢ��ֵ
    /// </summary>
    /// <param name="hashValue">ָ�����ɢ��ֵ��ָ��</param>
    /// <param name="buffer">������</param>
    /// <param name="startIndex">��ʼ�±�����������</param>
    /// <param name="endIndex">��ֹ�±�������������</param>
    D2_LANG_API void __stdcall d2lang_10001_GetDataHash(
        WORD* hashValue,
        BYTE* buffer,
        DWORD startIndex,
        DWORD endIndex
    );

#ifdef __cplusplus
}
#endif
