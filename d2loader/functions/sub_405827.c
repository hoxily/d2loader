#include <assert.h>
#include "sub_405827.h"
#include "sub_404ed0.h"
#include "sub_40798d.h"

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
    // 在汇编代码中，每次迭代时 esi 指向每个结构体对象的 +0x20 偏移处。
    assert(sizeof(struct patch_search_item) == 0x2c);
    for (struct patch_search_item* esi_ptr = items; esi_ptr->moduleFileName != (void*)-1; esi_ptr++, patchIndex++)
    {
        assert(offsetof(struct patch_search_item, v6) == 0x18);
        if (esi_ptr->v6 & 4)
        {
            continue;
        }

        var_18++;
        // 汇编代码里复用了实参 items 变量。此处声明一个新变量。
        // 汇编代码里反复计算基于hModule的偏移地址，声明为char*会更方便加减指针。
        char* address = (char*)GetModuleHandleA(esi_ptr->moduleFileName);
        if (address == NULL)
        {
            sub_404ed0_LogFormat(
                LOG_TAG,
                "Module %s not found",
                esi_ptr->moduleFileName
            );
            continue;
        }

        if (esi_ptr->v2 <= esi_ptr->v4_zero)
        {
            size = esi_ptr->v4_zero;
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

        address = address + esi_ptr->v1;
        sub_40798d(esi_ptr->v3, esi_ptr->v2);
    }
    return FALSE;//TODO
}