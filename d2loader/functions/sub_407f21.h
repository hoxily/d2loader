#pragma once

#include <Windows.h>

/*
* ʹ�ø������ַ�����s�е���һ�ַ����и�pattern�ַ�����
* @param pattern ���зֵ��ַ���
* @param s ���Էָ����ַ�����
* @param count �����и���ɵ��ַ��������Ԫ�ظ���
* @returns �и����ַ������顣�ַ���ָ�������Լ�ָ����ַ������Ѿ����������malloc�ڴ���ͷ�ʱֻ��free��ָ�뼴�ɡ�
*/
char** sub_407f21_SplitString(
    const char* pattern,
    const char* s,
    DWORD* count
);