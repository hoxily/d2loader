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
public:
    /*
    * 使用码元构造一个 Unicode。
    * 注意：提供默认值 0 时，会在DLL导出时多生成一个 Unicode::`default constructor closure'(void) 函数。
    * 这个 default constructor closure 函数也有它自己的函数体。
    * @param codeUnit 码元
    */
    Unicode(USHORT codeUnit = 0);
    /*
    * 重载赋值操作符
    */
    Unicode& operator =(const Unicode& ch);
    /*
    * 重载 (USHORT) 强制类型转换操作符。
    * 强制类型转换操作符不能写返回值类型。
    * 使用时 USHORT y = (USHORT)unicode; 即可。
    */
    operator USHORT() const;

    static void __fastcall Personalize(Unicode* dest, const Unicode* a, const Unicode* b, int i, ELANGUAGE lang);

    // 注意：带不带 const 限定，对于mangled name会产生影响，两者是不同的名字。
    int __thiscall compare(Unicode other) const;

    static int __fastcall compare(Unicode lhs, Unicode rhs);

private:
    static USHORT _toLowerTable[];

    static USHORT _toUpperTable[];

    USHORT m_codeUnit;
};