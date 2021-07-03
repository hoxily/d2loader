#include <Windows.h>
#include "sub_40a9a0.h"
#include <winternl.h>
#include "sub_404ed0.h"

void sub_40a9a0(
)
{
    PEB* edi_peb = NULL;
    /*
    * fs:30h 指向的是 Process Environment Block。
    * 由于windows api中没有直接获取该结构指针的函数，这里直接内联了汇编代码。
    * 
    * 参见下面的链接以了解关于PEB的更多信息：
    * 那些年病毒用过的损招——反调试技术: https://zhuanlan.zhihu.com/p/27841516
    * TEB structure: https://docs.microsoft.com/en-us/windows/win32/api/winternl/ns-winternl-teb
    * PEB structure: https://docs.microsoft.com/en-us/windows/win32/api/winternl/ns-winternl-peb
    * 
    * 参见下面的链接以了解如何在Microsoft Visual C/C++中内联汇编代码：
    * Inline Assembler Overview: https://docs.microsoft.com/en-us/cpp/assembler/inline/inline-assembler-overview?view=msvc-160
    */
    __asm
    {
        mov eax, fs:30h;
        mov edi_peb, eax;
    }

#pragma warning(disable: 6011)
    BOOL isBeingDebugged = edi_peb->BeingDebugged;
#pragma warning(default: 6011)

    if (isBeingDebugged)
    {
        sub_404ed0_LogFormat(
            LOG_TAG,
            "当前正在被调试器调试。"
        );
    }
    else
    {
        sub_404ed0_LogFormat(
            LOG_TAG,
            "当前没有检测到调试器。"
        );
    }
    //TODO
}
