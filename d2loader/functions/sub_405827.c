#include <assert.h>
#include "sub_405827.h"
#include "sub_404ed0.h"
#include "sub_40798d.h"
#include "sub_4076ab.h"
#include "sub_405b2d.h"
#include "sub_407889.h"
#include "sub_407778.h"
#include "sub_405b3b.h"

BOOL sub_405827_PatchModule(
    struct patch_search_item* items,
    int actionType
)
{
    if (items == NULL)
    {
        return FALSE;
    }

    int succeededPatchCount = 0;
    int totalPatchCount = 0;
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

        totalPatchCount++;
        // 汇编代码里复用了实参 items 变量。此处声明一个新变量。
        // 汇编代码里反复计算基于hModule的偏移地址，声明为char*会更方便加减指针。
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
        else
        {
            size = esi_ptr->v2;
        }

        if (actionType != PATCH_ACTION_TYPE_APPLY &&
            (
                (esi_ptr->v6 & 8) == 0 ||
                (esi_ptr->v6 & 0x40) != 0
            )
        )
        {
            succeededPatchCount++;
            continue;
        }

        // loc_4058b6

        address = address + esi_ptr->v1;
        void* memory = sub_40798d_DuplicateMemoryBlock(esi_ptr->v3, esi_ptr->v2);
        void* memory2 = sub_40798d_DuplicateMemoryBlock(esi_ptr->v5, esi_ptr->v4);
        if (memory == NULL || memory2 == NULL)
        {
            continue;
        }

        ptrdiff_t edi_offset = (char*)hModule - (char*)sub_4076ab_GetImageBase(hModule);
        if (edi_offset != 0)
        {
            if (esi_ptr->v2 >= 4 &&
                (esi_ptr->v6 & 1)
            )
            {
                sub_405b2d_IncreasePPtr(memory, edi_offset);
            }

            if (esi_ptr->v4 >= 4 &&
                (esi_ptr->v6 & 2)
            )
            {
                sub_405b2d_IncreasePPtr(memory2, edi_offset);
            }
        }

        // loc_405927
        if (esi_ptr->v6 & 0x20)
        {
            ptrdiff_t ecx_offset = (char*)0xfffffffcu - address;
            sub_405b2d_IncreasePPtr(memory2, ecx_offset);
        }

        if (actionType != PATCH_ACTION_TYPE_APPLY)
        { // restore patch
            int compare = sub_407889_smemcmp(
                memory2,
                address,
                esi_ptr->v4
            );

            if (compare != 0)
            {
                sub_404ed0_LogFormat(
                    LOG_TAG,
                    "Patch %d Source Data Mismatch, Using Search Result to Restore",
                    patchIndex
                );

                address = (char*)esi_ptr->patternLocation;
            }

            if (address != NULL && esi_ptr->v8 != NULL)
            {
                if (esi_ptr->v6 & 0x20)
                {
                    ptrdiff_t ecx_offset = (char*)0xfffffffcu - address;
                    sub_405b2d_IncreasePPtr(esi_ptr->v8, ecx_offset);
                }

                sub_407778_smemcpy(
                    address,
                    esi_ptr->v8,
                    esi_ptr->v4
                );

                if (esi_ptr->v6 & 0x10)
                {
                    free(esi_ptr->v8);
                    esi_ptr->v8 = NULL;
                    // clear lsb bit 4: 11111111111111111111111111101111
                    esi_ptr->v6 &= 0xffffffef;
                }
                
                // clear lsb bit 3: 11111111111111111111111111110111
                esi_ptr->v6 &= 0xfffffff7;

                succeededPatchCount++;
            }
        }
        else
        { // apply patch
            // loc_4059be
            if (esi_ptr->v6 & 8)
            {
                int compare = sub_407889_smemcmp(
                    memory,
                    address,
                    esi_ptr->v2
                );
                if (compare == 0)
                {
                    esi_ptr->patternLocation = NULL;
                }
                else
                {
                    sub_404ed0_LogFormat(
                        LOG_TAG,
                        "Patch %d Source Data Mismatch, Using Pattern Search",
                        patchIndex
                    );
                    address = (char*)sub_405b3b_SearchPattern(hModule, esi_ptr->pattern);
                    esi_ptr->patternLocation = address;
                }
            }
            else
            {
                succeededPatchCount++;
                address = NULL;
            }

            // loc_405a0a

            // 0x40 = 0b100 0000, test lsb bit 6
            if (esi_ptr->v6 & 0x40)
            {
                if (esi_ptr->v8 != NULL)
                {
                    sub_407778_smemcpy(
                        esi_ptr->v8,
                        address, // fix by hoxily@qq.com, 这里取了 address 的地址，而不是 address 存储的地址，很明显有问题。
                        4
                    );

                    succeededPatchCount++;
                }
            }
            else
            {
                // loc_405a29
                if (address != NULL)
                {
                    if (esi_ptr->v8 != NULL &&
                        (esi_ptr->v6 & 0x10) == 0
                    )
                    {
                        esi_ptr->v8 = malloc(size);
                        sub_407778_smemcpy(
                            esi_ptr->v8,
                            address,
                            size
                        );
                    }
                    else
                    {
                        sub_407778_smemcpy(
                            esi_ptr->v8,
                            address,
                            size
                        );
                    }

                    // loc_405a68
                    if ((esi_ptr->v6 & 0x20) &&
                        esi_ptr->v8 != NULL
                    )
                    {
                        ptrdiff_t ecx_offset = (ptrdiff_t)(address + 4);
                        sub_405b2d_IncreasePPtr(esi_ptr->v8, ecx_offset);
                    }

                    sub_407778_smemcpy(
                        address,
                        memory2,
                        esi_ptr->v4
                    );

                    // 0x8 = 0b1000, lsb bit 4
                    esi_ptr->v6 |= 0x8;

                    succeededPatchCount++;
                }
            }
        }

        free(memory);
        free(memory2);
    }

    const char* action = actionType != PATCH_ACTION_TYPE_APPLY ? "Restored" : "Applied";
    sub_404ed0_LogFormat(
        LOG_TAG,
        "%d of %d Patches Successfully %s",
        succeededPatchCount,
        totalPatchCount,
        action
    );

    return succeededPatchCount == totalPatchCount;
}