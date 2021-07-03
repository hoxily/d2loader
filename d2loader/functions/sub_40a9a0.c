#include <Windows.h>
#include "sub_40a9a0.h"
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
    //TODO
}
