#include <assert.h>
#include "sub_40532e.h"

IMAGE_IMPORT_DESCRIPTOR* sub_40532e(
    HMODULE hModule,
    const char* hookDll
)
{
    if (hookDll == NULL || hModule == NULL)
    {
        SetLastErrorEx(ERROR_INVALID_PARAMETER, 1);
        return NULL;
    }

    assert(sizeof(IMAGE_DOS_HEADER) == 0x40);
    if (IsBadReadPtr(hModule, sizeof(IMAGE_DOS_HEADER)))
    {
        SetLastErrorEx(ERROR_INVALID_PARAMETER, 1);
        return NULL;
    }

    WORD magicWord = *((WORD*)hModule);
    // DOS signature (MZ)
    if (magicWord != IMAGE_DOS_SIGNATURE)
    {
        SetLastErrorEx(ERROR_INVALID_PARAMETER, 1);
        return NULL;
    }

    IMAGE_DOS_HEADER* header = (IMAGE_DOS_HEADER*)hModule;
    assert(offsetof(IMAGE_DOS_HEADER, e_lfanew) == 0x3c);
    LONG aNew = header->e_lfanew;
    void* newPtr = (char*)hModule + aNew;

    assert(sizeof(IMAGE_NT_HEADERS32) == 0xf8);
    if (IsBadReadPtr(newPtr, sizeof(IMAGE_NT_HEADERS32)))
    {
        SetLastErrorEx(ERROR_INVALID_PARAMETER, 1);
        return NULL;
    }
    IMAGE_NT_HEADERS32* ntHeader = (IMAGE_NT_HEADERS32*)newPtr;
    if (ntHeader->Signature != IMAGE_NT_SIGNATURE)
    {
        SetLastErrorEx(ERROR_INVALID_PARAMETER, 1);
        return NULL;
    }
    char* ptr_0x80 = (char*)&ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
    char* ptr_0 = (char*)ntHeader;
    assert(ptr_0x80 - ptr_0 == 0x80);

    IMAGE_DATA_DIRECTORY importDataDirectory = ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
    if (importDataDirectory.VirtualAddress == 0)
    {
        return NULL;
    }

    IMAGE_IMPORT_DESCRIPTOR* importDescriptor = (IMAGE_IMPORT_DESCRIPTOR*)((char*)hModule + importDataDirectory.VirtualAddress);
    while (importDescriptor->Name)
    {
        const char* functionName = (const char*)hModule + importDescriptor->Name;
        if (lstrcmpiA(functionName, hookDll) == 0)
        {
            break;
        }

        importDescriptor++;
    }
    /*
    * 这里有点妙。无跳转的三元运算符实现。
    *
    * esi 当前为 importDescriptor 指针
    * mov eax, [esi + 0ch]
    * neg eax
    * sbb eax, eax
    * and eax, esi
    * ...
    * retn
    *
    * 这里的neg、sbb组合与上次遇到的情况一样。都是将某个数，变为0或者0xffffffff。
    * 最后 and 运算，就相当于根据 importDescriptor->Name 是否为 0，来决定要返回 importDescriptor 还是 NULL。
    */

    return importDescriptor->Name ? importDescriptor : NULL;
}
