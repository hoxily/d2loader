#include "pch.h"
#include "sub_40a91f.h"
#include "sub_404ed0.h"

void sub_40a91f(
)
{
    sub_404ed0_LogFormat(
        LOG_TAG,
        "patch d2 program to alt color"
    );

    DWORD oldProtect;
    VirtualProtect(
        (void*)0x6F8F34A4,
        4,
        PAGE_EXECUTE_READWRITE,
        &oldProtect
    );
    *(DWORD*)0x6F8F34A4 = 0x40a91e;// loc_40a91e ����һ�� cdq ָ��������ܹ֡��ѵ�ֻ���븳�����ֳ��� 0x40a91e ��
    DWORD unused;
    VirtualProtect(
        (void*)0x6F8F34A4,
        4,
        oldProtect,
        &unused
    );

    VirtualProtect(
        (void*)0x6F8F34FD,
        1,
        PAGE_EXECUTE_READWRITE,
        &oldProtect
    );
    *(BYTE*)0x6F8F34FE = 4;
    VirtualProtect(
        (void*)0x6F8F34FD,
        4,
        oldProtect,
        &unused
    );
}