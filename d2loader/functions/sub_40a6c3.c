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

    // �Թ� sub_40a814()��ֱ�ӽ�������Ƕ�뵽�˺�����
    *(DWORD*)0x6F8F8A5F = 0x90909090;
    *(WORD*)0x6F8F8A62 = 0x16a; // �����������һ�����ص�������֡�

    // ����ָ�������������޸Ĳ���������û����ԡ�����֡�
    /*
    VirtualProtect(
        0x6F8EC361,
        1,
        oldProtect,
        &unused
    );
    */
    // fix by hoxily@qq.com��ʹ VirtualProtect ��ԡ�
    VirtualProtect(
        (void*)0x6F8F8A5F,
        5,
        oldProtect,
        &unused
    );
}