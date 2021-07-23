#include "pch.h"
#include "sub_40a6c3.h"

void sub_40a6c3(
)
{
    DWORD oldProtect;
    VirtualProtect(
        (void*)0x6FAF3989,
        1,
        PAGE_EXECUTE_READWRITE,
        &oldProtect
    );

    *(BYTE*)0x6FAF3989 = 1;

    DWORD unused;
    VirtualProtect(
        (void*)0x6FAF3989,
        1,
        oldProtect,
        &unused
    );

    VirtualProtect(
        (void*)0x6F8F8A5F,
        5,
        PAGE_EXECUTE_READWRITE,
        &oldProtect
    );

    // 略过 sub_40a814()，直接将函数体嵌入到此函数。
    *(DWORD*)0x6F8F8A5F = 0x90909090;
    *(WORD*)0x6F8F8A62 = 0x16a; // 这个与上面这一行有重叠。很奇怪。

    // 这个恢复操作与上面的修改操作看起来没有配对。很奇怪。
    /*
    VirtualProtect(
        0x6F8EC361,
        1,
        oldProtect,
        &unused
    );
    */
    // fix by hoxily@qq.com，使 VirtualProtect 配对。
    VirtualProtect(
        (void*)0x6F8F8A5F,
        5,
        oldProtect,
        &unused
    );
}