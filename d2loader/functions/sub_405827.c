#include <assert.h>
#include "sub_405827.h"
#include "sub_404ed0.h"
#include "sub_40798d.h"
#include "sub_4076ab.h"
#include "sub_405b2d.h"

BOOL sub_405827_PatchModule(
    struct patch_search_item* items,
    int arg1
)
{
    if (items == NULL)
    {
        return FALSE;
    }

    int var_4 = 0;
    int var_18 = 0;
    int patchIndex = 0;
    DWORD size;
    // �ڻ������У�ÿ�ε���ʱ esi ָ��ÿ���ṹ������ +0x20 ƫ�ƴ���
    assert(sizeof(struct patch_search_item) == 0x2c);
    for (struct patch_search_item* esi_ptr = items; esi_ptr->moduleFileName != (void*)-1; esi_ptr++, patchIndex++)
    {
        assert(offsetof(struct patch_search_item, v6) == 0x18);
        if (esi_ptr->v6 & 4)
        {
            continue;
        }

        var_18++;
        // �������︴����ʵ�� items �������˴�����һ���±�����
        // �������ﷴ���������hModule��ƫ�Ƶ�ַ������Ϊchar*�������Ӽ�ָ�롣
        HMODULE hModule = GetModuleHandleA(esi_ptr->moduleFileName);
        char* address = (char*)hModule;
        if (address == NULL)
        {
            sub_404ed0_LogFormat(
                LOG_TAG,
                "Module %s not found",
                esi_ptr->moduleFileName
            );
            continue;
        }

        if (esi_ptr->v2 <= esi_ptr->v4)
        {
            size = esi_ptr->v4;
        }

        if (arg1 != 0 &&
            (
                (esi_ptr->v6 & 8) == 0 ||
                (esi_ptr->v6 & 0x40) != 0
            )
        )
        {
            var_4++;
            continue;
        }

        // loc_4058b6

        address = address + esi_ptr->v1;
        void* memory = sub_40798d_DuplicateMemoryBlock(esi_ptr->v3, esi_ptr->v2);
        void* var_8 = sub_40798d_DuplicateMemoryBlock(esi_ptr->v5, esi_ptr->v4);
        if (memory == NULL || var_8 == NULL)
        {
            continue;
        }

        ptrdiff_t edi_offset = (char*)hModule - (char*)sub_4076ab(hModule);
        if (edi_offset != 0)
        {
            if (esi_ptr->v2 >= 4 &&
                (esi_ptr->v6 & 1)
            )
            {
                sub_405b2d_IncreasePPtr(memory, edi_offset);
            }
        }
    }
    return FALSE;//TODO
}