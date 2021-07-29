#include "pch.h"
#include "d2lang-10000.h"

unsigned int __stdcall d2lang_10000_HashString(
    const char* str,
    unsigned int n
)
{
    unsigned int eax_hash = 0;
    for (const char* edx_ptr = str; *edx_ptr != '\0'; edx_ptr++)
    {
        // movsx ecx, cl 表示保留符号位做扩展。
        // 不加 (int) 也是movsx。
        // (unsigned char) 才会变成 movzx
        unsigned int ecx = (int)*edx_ptr;
        eax_hash <<= 4;
        eax_hash += ecx;
        ecx = eax_hash;
        ecx &= 0xF0000000;
        if (ecx != 0)
        {
            ecx >>= 0x18;
            ecx ^= eax_hash;
            ecx &= 0x0FFFFFFF;
            eax_hash = ecx;
        }
    }
    return eax_hash % n;
}