#pragma once

#include "pch.h"
#include "D2LangApi.h"
#include "data-types.h"

/*
* 需要在这里使用 struct __declspec(dllexport) 的方式导出Unicode类。如果仅仅使用 .def 文件，
* 则会找不到 Unicode::`default constructor closure'(void) 这个符号。
*/
struct D2_LANG_API Unicode
{
    /*
    * Unicode字符的方向性
    */
    enum class Direction
    {
        LeftToRight = 0xf0,
        RightToLeft = 0xf1,
        Didirectional = 0xf2,
    };

public:
    /*
    * 使用码元构造一个 Unicode。
    * 注意：提供默认值 0 时，会在DLL导出时多生成一个 Unicode::`default constructor closure'(void) 函数。
    * 这个 default constructor closure 函数也有它自己的函数体。
    * @param codeUnit 码元
    */
    __thiscall Unicode(USHORT codeUnit = 0);
    /*
    * 重载赋值操作符
    */
    Unicode& __thiscall operator =(const Unicode& ch);
    /*
    * 重载 (USHORT) 强制类型转换操作符。
    * 强制类型转换操作符不能写返回值类型。
    * 使用时 USHORT y = (USHORT)unicode; 即可。
    */
    __thiscall operator USHORT() const;

    static void __fastcall Personalize(Unicode* dest, const Unicode* a, const Unicode* b, int i, ELANGUAGE lang);

    // 注意：带不带 const 限定，对于mangled name会产生影响，两者是不同的名字。
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