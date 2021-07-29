#pragma once

#include "pch.h"
#include "D2LangApi.h"
#include "data-types.h"

/*
* ��Ҫ������ʹ�� struct __declspec(dllexport) �ķ�ʽ����Unicode�ࡣ�������ʹ�� .def �ļ���
* ����Ҳ��� Unicode::`default constructor closure'(void) ������š�
*/
struct D2_LANG_API Unicode
{
    /*
    * Unicode�ַ��ķ�����
    */
    enum class Direction
    {
        LeftToRight = 0xf0,
        RightToLeft = 0xf1,
        Didirectional = 0xf2,
    };

public:
    /*
    * ʹ����Ԫ����һ�� Unicode��
    * ע�⣺�ṩĬ��ֵ 0 ʱ������DLL����ʱ������һ�� Unicode::`default constructor closure'(void) ������
    * ��� default constructor closure ����Ҳ�����Լ��ĺ����塣
    * @param codeUnit ��Ԫ
    */
    __thiscall Unicode(USHORT codeUnit = 0);
    /*
    * ���ظ�ֵ������
    */
    Unicode& __thiscall operator =(const Unicode& ch);
    /*
    * ���� (USHORT) ǿ������ת����������
    * ǿ������ת������������д����ֵ���͡�
    * ʹ��ʱ USHORT y = (USHORT)unicode; ���ɡ�
    */
    __thiscall operator USHORT() const;

    static void __fastcall Personalize(Unicode* dest, const Unicode* a, const Unicode* b, int i, ELANGUAGE lang);

    // ע�⣺������ const �޶�������mangled name�����Ӱ�죬�����ǲ�ͬ�����֡�
    int __thiscall compare(Unicode other) const;

    static int __fastcall compare(Unicode lhs, Unicode rhs);

    Direction __thiscall directionality();

    BOOL __thiscall isASCII() const;

    BOOL __thiscall isAlpha() const;

private:
    static USHORT _toLowerTable[];

    static USHORT _toUpperTable[];

    USHORT m_codeUnit;
};