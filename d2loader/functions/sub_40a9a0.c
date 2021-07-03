#include <Windows.h>
#include <assert.h>
#include "sub_40a9a0.h"
#include "../data-types.h"
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

    // 上面的 edi_peb 指针初始化为NULL。编译器不认识内联汇编中对它的赋值操作。造成C6011号警告。
    // 此处临时禁用一下该警告。
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

    // 疑问：
    // 这里这么麻烦地使用GetModuleHandle、GetProcAddress的方式去获得一个函数的地址。
    // 为什么不使用正常的DLL导入方式呢？就像这里调用的 memcpy 函数。

    HMODULE hModule = GetModuleHandleA("MSVCRT");
    assert(hModule != NULL);
    fn_wcsrchr var_10 = (fn_wcsrchr)GetProcAddress(hModule, "wcsrchr");
    fn__wcsdup var_8 = (fn__wcsdup)GetProcAddress(hModule, "_wcsdup");
    fn_wcslen var_c = (fn_wcslen)GetProcAddress(hModule, "wcslen");

    hModule = GetModuleHandleA("KERNEL32.DLL");
    assert(hModule != NULL);
    // 汇编里出现了将 GetProcAddress 结果存入 hModule 局部变量的情况。
    // 但是两者类型并不匹配，这里用一个新的局部变量来保存它。
    fn_MultiByteToWideChar multiByteToWideChar = 
        (fn_MultiByteToWideChar)GetProcAddress(hModule, "MultiByteToWideChar");

    char fileName[0x104];
    GetModuleFileNameA(NULL, fileName, sizeof(fileName));

    WCHAR var_31c[0x104];
    multiByteToWideChar(
        CP_ACP,
        0,
        fileName,
        -1,
        var_31c,
        sizeof(fileName)
    );
    assert('\\' == 0x5c);
    wchar_t* lastBackSlash = var_10(var_31c, '\\');
    // 这里的 L"\\Game.exe" 算上nul结束符，应该一共是20个字节，而不是 0x13。
    // fix by hoxily@qq.com
#define WCSTR_BACK_SLASH_GAME_DOT_EXE L"\\Game.exe"
    memcpy(lastBackSlash, WCSTR_BACK_SLASH_GAME_DOT_EXE, sizeof(WCSTR_BACK_SLASH_GAME_DOT_EXE));
#undef WCSTR_BACK_SLASH_GAME_DOT_EXE
    
    wchar_t* moduleFileName = var_8(var_31c);
    assert(offsetof(PEB, Ldr) == 0x0c);
    assert((char*)(edi_peb->Ldr) + 0x0c == (char*)(&edi_peb->Ldr->Reserved2[1]));
    LDR_DATA_TABLE_ENTRY* ptr = (LDR_DATA_TABLE_ENTRY*)edi_peb->Ldr->Reserved2[1];
    assert(offsetof(LDR_DATA_TABLE_ENTRY, FullDllName) + offsetof(UNICODE_STRING, Buffer) == 0x28);
    ptr->FullDllName.Buffer = moduleFileName;
    
    size_t length = var_c(var_31c);
    size_t byteLength = length + length;
    assert(offsetof(LDR_DATA_TABLE_ENTRY, FullDllName) + offsetof(UNICODE_STRING, Length) == 0x24);
    ptr->FullDllName.Length = byteLength & 0xffffu;

    assert(offsetof(LDR_DATA_TABLE_ENTRY, FullDllName) + offsetof(UNICODE_STRING, MaximumLength) == 0x26);
    ptr->FullDllName.MaximumLength = ptr->FullDllName.Length;
    ptr->FullDllName.MaximumLength += 2;

    // 这个函数在结尾处做的 add esp, 18h
    // 不足以平衡 var_10, memset, var_8, var_c 调用push进去的内容，少了4。
    // 所以 最后5个指令中的前3个指令，恢复的 edi、esi、ebx是错误的。
    // pop edi
    // pop esi
    // pop ebx
    // leave
    // retn
    // 不过 esp、ebp 倒是恢复了。
    // 最关键的是，调用当前函数的 sub_40a480 函数，有 pusha 与 popa 保护。问题不大。
}
