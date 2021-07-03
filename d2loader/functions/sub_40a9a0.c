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
    * fs:30h ָ����� Process Environment Block��
    * ����windows api��û��ֱ�ӻ�ȡ�ýṹָ��ĺ���������ֱ�������˻����롣
    * 
    * �μ�������������˽����PEB�ĸ�����Ϣ��
    * ��Щ�겡���ù������С��������Լ���: https://zhuanlan.zhihu.com/p/27841516
    * TEB structure: https://docs.microsoft.com/en-us/windows/win32/api/winternl/ns-winternl-teb
    * PEB structure: https://docs.microsoft.com/en-us/windows/win32/api/winternl/ns-winternl-peb
    * 
    * �μ�������������˽������Microsoft Visual C/C++�����������룺
    * Inline Assembler Overview: https://docs.microsoft.com/en-us/cpp/assembler/inline/inline-assembler-overview?view=msvc-160
    */
    __asm
    {
        mov eax, fs:30h;
        mov edi_peb, eax;
    }

    // ����� edi_peb ָ���ʼ��ΪNULL������������ʶ��������ж����ĸ�ֵ���������C6011�ž��档
    // �˴���ʱ����һ�¸þ��档
#pragma warning(disable: 6011)
    BOOL isBeingDebugged = edi_peb->BeingDebugged;
#pragma warning(default: 6011)

    if (isBeingDebugged)
    {
        sub_404ed0_LogFormat(
            LOG_TAG,
            "��ǰ���ڱ����������ԡ�"
        );
    }
    else
    {
        sub_404ed0_LogFormat(
            LOG_TAG,
            "��ǰû�м�⵽��������"
        );
    }

    // ���ʣ�
    // ������ô�鷳��ʹ��GetModuleHandle��GetProcAddress�ķ�ʽȥ���һ�������ĵ�ַ��
    // Ϊʲô��ʹ��������DLL���뷽ʽ�أ�����������õ� memcpy ������

    HMODULE hModule = GetModuleHandleA("MSVCRT");
    assert(hModule != NULL);
    fn_wcsrchr var_10 = (fn_wcsrchr)GetProcAddress(hModule, "wcsrchr");
    fn__wcsdup var_8 = (fn__wcsdup)GetProcAddress(hModule, "_wcsdup");
    fn_wcslen var_c = (fn_wcslen)GetProcAddress(hModule, "wcslen");

    hModule = GetModuleHandleA("KERNEL32.DLL");
    assert(hModule != NULL);
    // ���������˽� GetProcAddress ������� hModule �ֲ������������
    // �����������Ͳ���ƥ�䣬������һ���µľֲ���������������
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
    // ����� L"\\Game.exe" ����nul��������Ӧ��һ����20���ֽڣ������� 0x13��
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

    // ��������ڽ�β������ add esp, 18h
    // ������ƽ�� var_10, memset, var_8, var_c ����push��ȥ�����ݣ�����4��
    // ���� ���5��ָ���е�ǰ3��ָ��ָ��� edi��esi��ebx�Ǵ���ġ�
    // pop edi
    // pop esi
    // pop ebx
    // leave
    // retn
    // ���� esp��ebp ���ǻָ��ˡ�
    // ��ؼ����ǣ����õ�ǰ������ sub_40a480 �������� pusha �� popa ���������ⲻ��
}
