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

    BOOL __thiscall isLeftToRight() const;

    static BOOL __fastcall isLineBreak(const Unicode* str, unsigned int u);

    BOOL __thiscall isNewline() const;

    BOOL __thiscall isPipe() const;

    BOOL __thiscall isWhitespace() const;

    static BOOL __fastcall isWordEnd(const Unicode* str, unsigned int u);

    static BOOL __fastcall loadSysMap(HD2ARCHIVE__* mpqHandle, const char* str);

    static void __cdecl sprintf(int bufferSize, Unicode* buffer, const Unicode* format, ...);

    static Unicode* __fastcall strcat(Unicode* dest, const Unicode* src);

    static Unicode* __fastcall strchr(const Unicode* str, Unicode ch);

    static int __fastcall strcmp(const Unicode* lhs, const Unicode* rhs);

    static int __fastcall strcoll(const Unicode* lhs, const Unicode* rhs);

    static Unicode* __fastcall strcpy(Unicode* dest, const Unicode* src);

    static int __fastcall stricmp(const Unicode* lhs, const Unicode* rhs);

    static int __fastcall strlen(const Unicode* str);

    static Unicode* __fastcall strncat(Unicode* dest, const Unicode* src, int n);

    static int __fastcall strncmp(const Unicode* lhs, const Unicode* rhs, unsigned int n);

    static int __fastcall strncoll(const Unicode* lhs, const Unicode* rhs, int n);

    static Unicode* __fastcall strncpy(Unicode* dest, const Unicode* src, int n);

    static int __fastcall strnicmp(const Unicode* lhs, const Unicode* rhs, unsigned int n);

    static Unicode* __fastcall strstr(const Unicode* str, const Unicode* subStr);

    static Unicode* __fastcall strstri(const Unicode* str, const Unicode* subStr);

    static Unicode* __fastcall sys2Unicode(Unicode* buffer, const char* str, int bufferSize);

    static unsigned long __fastcall sysWidth(const Unicode* str, int i);

    Unicode __thiscall toLower() const;

    static Unicode* __fastcall toUnicode(Unicode* buffer, const char* str, int bufferSize);

    Unicode __thiscall toUpper() const;

    static char* __fastcall toUtf(char* buffer, const Unicode* str, int bufferSize);

    static char* __fastcall unicode2Sys(char* buffer, const Unicode* str, int bufferSize);

    static char* __fastcall unicode2Win(char* buffer, const Unicode* str, int bufferSize);

    static unsigned long __fastcall unicodeWidth(const char* str, int i);

    static unsigned int __fastcall unicodenwidth(const char* str, int n);

    static void __fastcall unloadSysMap();

    static Unicode* __fastcall utf8ToUnicode(Unicode* buffer, const char* str, int bufferSize);

    static unsigned int __fastcall utfnwidth(const Unicode* str, int n);

    int __thiscall utfwidth() const;

    static Unicode* __fastcall win2Unicode(Unicode* buffer, const char* str, int bufferSize);

private:
    static USHORT _toLowerTable[];

    static USHORT _toUpperTable[];

    USHORT m_codeUnit;
};