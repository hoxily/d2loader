#include "pch.h"
#include "unicode.h"

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
    //TODO
}

int Unicode::compare(Unicode other) const
{
    //TODO
    return 0;
}

int Unicode::compare(Unicode lhs, Unicode rhs)
{
    //TODO
    return 0;
}

Unicode::Direction Unicode::directionality()
{
    //TODO
    return Direction::LeftToRight;
}

BOOL Unicode::isASCII() const
{
    //TODO
    return false;
}

BOOL Unicode::isAlpha() const
{
    //TODO
    return false;
}

BOOL Unicode::isLeftToRight() const
{
    //TODO
    return false;
}

BOOL Unicode::isLineBreak(const Unicode* str, unsigned int u)
{
    //TODO
    return false;
}

BOOL Unicode::isNewline() const
{
    //TODO
    return false;
}

BOOL Unicode::isPipe() const
{
    //TODO
    return false;
}

BOOL Unicode::isWhitespace() const
{
    //TODO
    return false;
}

BOOL Unicode::isWordEnd(const Unicode* str, unsigned int u)
{
    //TODO
    return false;
}

BOOL Unicode::loadSysMap(HD2ARCHIVE__* mpqHandle, const char* str)
{
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
    //TODO
    return 0;
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
    //TODO
    return *this;
}

Unicode* Unicode::toUnicode(Unicode* buffer, const char* str, int bufferSize)
{
    //TODO
    return buffer;
}

Unicode Unicode::toUpper() const
{
    //TODO
    return *this;
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