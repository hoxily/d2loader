#pragma once

#include "../D2LangApi.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
* ��ȡ�ַ�����Hashֵ������ֵ��ΧΪ [0, n - 1]��
* @param str ��Ҫ��Hashֵ���ַ���
* @param n �޶�����ֵ��ΧΪ [0, n - 1]
* @returns ���� [0, n - 1] ��Χ��Hashֵ��
*/
D2_LANG_API unsigned int __stdcall d2lang_10000_GetStringHash(
    const char* str,
    unsigned int n
);

#ifdef __cplusplus
}
#endif
