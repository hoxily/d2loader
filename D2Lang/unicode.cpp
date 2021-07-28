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