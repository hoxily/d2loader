#pragma once

#include "pch.h"

/*
* ʹ�ø������ַ����� separators �е���һ�ַ����и� str �ַ�����
* @param str ���зֵ��ַ���
* @param separators ���Էָ����ַ�����
* @param count �����и���ɵ��ַ��������Ԫ�ظ���������ΪNULL��
* @returns �и����ַ������飬��һ��NULL�������ַ���ָ�������Լ�ָ����ַ������Ѿ����������malloc�ڴ���ͷ�ʱֻ��free��ָ�뼴�ɡ�
*/
char** sub_407f21_SplitString(
    const char* str,
    const char* separators,
    DWORD* count
);