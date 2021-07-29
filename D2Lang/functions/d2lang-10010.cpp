#include "pch.h"
#include "d2lang-10010.h"

DWORD __stdcall d2lang_10010(
    DWORD arg0,
    DWORD* arg1
)
{
    *arg1 = arg0 + rand();
    //TODO
    return 0;
}