#include "pch.h"
#include "unicode.h"
#include <cassert>
#include "functions/sub_6fc01301.h"
#include "functions/d2lang-10009.h"
#include "functions/sub_6fc01340.h"
#include "functions/sub_6fc0137a.h"
#include "fog-api.h"
#include "functions/sub_6fc07ef0.h"
#include "functions/sub_6fc01580.h"

Unicode::Unicode(USHORT codeUnit)
{
    this->m_codeUnit = codeUnit;
}

Unicode& Unicode::operator=(const Unicode& ch)
{
    this->m_codeUnit = ch.m_codeUnit;
    return *this;
}

Unicode::operator USHORT() const
{
    return this->m_codeUnit;
}

void Unicode::Personalize(Unicode* dest, const Unicode* a, const Unicode* b, int i, ELANGUAGE lang)
{
    const Unicode* esi_ptr = a;
    Unicode* edi_ptr = dest;
    int aLength = 0;
    int bLength = 0;
    if (esi_ptr != nullptr)
    {
        aLength = Unicode::strlen(esi_ptr);
    }
    const Unicode* ebp_ptr = b;
    if (ebp_ptr != nullptr)
    {
        bLength = Unicode::strlen(ebp_ptr);
    }

    if (aLength + bLength + 5 > i)
    {
        if (bLength >= i)
        {
            return;
        }
        else
        {
            const Unicode* eax = b;
            Unicode ch;
            assert(eax != nullptr);
            do
            {
                ch = *eax;
                *dest = ch;

                dest++;
                eax++;
            } while (ch.m_codeUnit != 0);

            return;
        }
    }

    Unicode str[10];
    memset(str, 0, sizeof(str));

    //TODO
    switch (lang)
    {
    case ELANGUAGE::ESP:
        break;
    case ELANGUAGE::FRA:
        break;
    case ELANGUAGE::ITA:
        break;
    case ELANGUAGE::ENG_DEFAULT:
    case ELANGUAGE::ENG:
        break;
    case ELANGUAGE::DEU:
        break;
    case ELANGUAGE::POL:
        break;
    default:
        break;
    }
}

int Unicode::compare(Unicode other) const
{
    return Unicode::compare(*this, other);
}

int Unicode::compare(Unicode lhs, Unicode rhs)
{
    unsigned short lCode = lhs.m_codeUnit;
    if (lCode < 256)
    {
        lCode = Unicode::_toUpperTable[lCode];
    }

    unsigned short rCode = rhs.m_codeUnit;
    if (rCode < 256)
    {
        rCode = Unicode::_toUpperTable[rCode];
    }

    return lCode == rCode;
}

Unicode::Direction Unicode::directionality()
{
    //TODO
    return Direction::LeftToRight;
}

BOOL Unicode::isASCII() const
{
    // cmp word ptr[ecx], 80h
    // sbb eax, eax
    // neg eax
    // 注意，neg eax 指令指的是 eax = 0 - eax，
    // 相当于 eax = (not eax) + 1
    // 先前把它当成了 not 指令，所以引起了错误的判断。
    // 原始代码并没有错误。
    return this->m_codeUnit < 0x80;
}

BOOL Unicode::isAlpha() const
{
    if (this->m_codeUnit >= 0x80)
    {
        return false;
    }

    bool isAlphabet = sub_6fc01301_IsAlphabet(this->m_codeUnit);
    return isAlphabet;
}

BOOL Unicode::isLeftToRight() const
{
    unsigned short code = this->m_codeUnit;
    if (code >= 0x590 && code <= 0x5ff)
    {
        return false;
    }

    if (code < 0x600)
    {
        return true;
    }

    if (code > 0x6ff)
    {
        return true;
    }

    return false;
}

BOOL Unicode::isLineBreak(const Unicode* str, unsigned int u)
{
    if (u == 0)
    {
        return true;
    }

    ELANGUAGE ret = d2lang_10009_GetLanguage();
    if (ret == ELANGUAGE::JPN ||
        ret == ELANGUAGE::SIN ||
        ret == ELANGUAGE::CHI
        )
    {
        // loc_6fc086a1
        unsigned short code = str[u];
        if (code == 0x2026 // …
            || code == 0x3001 // 、
            || code == 0x3002 // 。
            || code == 0xff01 // ！
            || code == 0xff0c // ，
            || code == 0xff0e // ．
            || code == 0xff1f // ？
            || code == 0xff61 // ｡
            || code == 0xff64 // ､
            )
        {
            return false;
        }
        if (code >= 0x61 && code <= 0x7a) // 半角小写英文字母：'a' to 'z'
        {
            return false;
        }
        if (code >= 0xff41 && code <= 0xff5a) // 全角小写英文字母：'ａ' to 'ｚ'
        {
            return false;
        }
        if (code >= 0x41 && code <= 0x5a) // 半角大写英文字母: 'A' to 'Z'
        {
            return false;
        }
        if (code >= 0xff21 && code <= 0xff3a) // 全角大写英文字母：'Ａ' to 'Ｚ'
        {
            return false;
        }
        if (code >= 0x30 && code <= 0x39) // 阿拉伯数字：'0' to '9'
        {
            return false;
        }
        if (code >= 0xff10 && code <= 0xff19) // 全角阿拉伯数字：'０' to '９'
        {
            return false;
        }
        
        return true;
    }
    else
    {
        // loc_6fc08656
        unsigned short code = str[u];
        if (code < 0x100)
        {
            if (sub_6fc01340(code))
            {
                return false;
            }
        }
        code = str[u - 1];
        if (code < 0x100)
        {
            if (sub_6fc01340(code))
            {
                return true;
            }
        }
        return false;
    }
}

BOOL Unicode::isNewline() const
{
    return this->m_codeUnit == '\n';
}

BOOL Unicode::isPipe() const
{
    if (this->m_codeUnit >= 0x100)
    {
        return false;
    }

    return this->m_codeUnit == 0xff; // 'ÿ'
}

BOOL Unicode::isWhitespace() const
{
    unsigned short code = this->m_codeUnit;
    if (code >= 0x100)
    {
        return false;
    }

    return sub_6fc01340(code);
}

BOOL Unicode::isWordEnd(const Unicode* str, unsigned int u)
{
    if (str == nullptr)
    {
        return false;
    }

    if (sub_6fc0137a(str[u].m_codeUnit) &&
        !sub_6fc0137a(str[u + 1].m_codeUnit)
        )
    {
        return true;
    }
    else
    {
        return false;
    }
}

BOOL Unicode::loadSysMap(HD2ARCHIVE__* mpqHandle, const char* str)
{
    void* edi_ptr = sub_6fc07ef0(mpqHandle, str, 0);
    if (sub_6fc01580(edi_ptr, "KAMAP\n", 5))
    {
        fog_10043(edi_ptr, __FILE__, __LINE__, 0);
        return false;
    }

    //TODO
    return false;
}

void Unicode::sprintf(int bufferSize, Unicode* buffer, const Unicode* format, ...)
{
    //TODO
}

Unicode* Unicode::strcat(Unicode* dest, const Unicode* src)
{
    //TODO
    return dest;
}

Unicode* Unicode::strchr(const Unicode* str, Unicode ch)
{
    //TODO
    return nullptr;
}

int Unicode::strcmp(const Unicode* lhs, const Unicode* rhs)
{
    //TODO
    return 0;
}

int Unicode::strcoll(const Unicode* lhs, const Unicode* rhs)
{
    //TODO
    return 0;
}

Unicode* Unicode::strcpy(Unicode* dest, const Unicode* src)
{
    //TODO
    return dest;
}

int Unicode::stricmp(const Unicode* lhs, const Unicode* rhs)
{
    //TODO
    return 0;
}

int Unicode::strlen(const Unicode* str)
{
    if (str == nullptr)
    {
        return 0;
    }

    int length = 0;
    while (str[length].m_codeUnit != 0)
    {
        length++;
    }
    return length;
}

Unicode* Unicode::strncat(Unicode* dest, const Unicode* src, int n)
{
    //TODO
    return dest;
}

int Unicode::strncmp(const Unicode* lhs, const Unicode* rhs, unsigned int n)
{
    //TODO
    return 0;
}

int Unicode::strncoll(const Unicode* lhs, const Unicode* rhs, int n)
{
    //TODO
    return 0;
}

Unicode* Unicode::strncpy(Unicode* dest, const Unicode* src, int n)
{
    //TODO
    return dest;
}

int Unicode::strnicmp(const Unicode* lhs, const Unicode* rhs, unsigned int n)
{
    //TODO
    return 0;
}

Unicode* Unicode::strstr(const Unicode* str, const Unicode* subStr)
{
    //TODO
    return nullptr;
}

Unicode* Unicode::strstri(const Unicode* str, const Unicode* subStr)
{
    //TODO
    return nullptr;
}

Unicode* Unicode::sys2Unicode(Unicode* buffer, const char* str, int bufferSize)
{
    //TODO
    return buffer;
}

unsigned long Unicode::sysWidth(const Unicode* str, int i)
{
    //TODO
    return 0;
}

Unicode Unicode::toLower() const
{
    /*
    * 虽然 toLower 是无参数的 __thiscall 函数。但是因为要返回的是一个结构体，所以
    * 在编译器编译后，依然会添加一个参数，相当于调用方分配的 Unicode 结构的存储空间，把返回结果
    * 的指针传入给 toLower 函数。
    */
    unsigned short code = this->m_codeUnit;
    if (code < 256)
    {
        code = Unicode::_toLowerTable[code];
    }
    return Unicode(code);
}

Unicode* Unicode::toUnicode(Unicode* buffer, const char* str, int bufferSize)
{
    //TODO
    return buffer;
}

Unicode Unicode::toUpper() const
{
    // toUpper 与 toLower 同理，在汇编代码里会发现传入了一个用于返回 Unicode 的指针。
    unsigned short code = this->m_codeUnit;
    if (code < 256)
    {
        code = Unicode::_toUpperTable[code];
    }
    return Unicode(code);
}

char* Unicode::toUtf(char* buffer, const Unicode* str, int bufferSize)
{
    //TODO
    return buffer;
}

char* Unicode::unicode2Sys(char* buffer, const Unicode* str, int bufferSize)
{
    //TODO
    return buffer;
}

char* Unicode::unicode2Win(char* buffer, const Unicode* str, int bufferSize)
{
    //TODO
    return buffer;
}

unsigned long Unicode::unicodeWidth(const char* str, int i)
{
    //TODO
    return 0;
}

unsigned int Unicode::unicodenwidth(const char* str, int n)
{
    //TODO
    return 0;
}

void Unicode::unloadSysMap()
{
    //TODO
}

Unicode* Unicode::utf8ToUnicode(Unicode* buffer, const char* str, int bufferSize)
{
    //TODO
    return buffer;
}

unsigned int Unicode::utfnwidth(const Unicode* str, int n)
{
    //TODO
    return 0;
}

int Unicode::utfwidth() const
{
    //TODO
    return 0;
}

Unicode* Unicode::win2Unicode(Unicode* buffer, const char* str, int bufferSize)
{
    //TODO
    return buffer;
}