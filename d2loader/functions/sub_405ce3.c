#include <assert.h>
#include "sub_405ce3.h"

/*
* 这个函数使用了 edx 寄存器，但是并没有用一对 push edx、popd edx 来保护它。
* 按照 Intel Application Binary Interface 规范，eax、ecx、edx 这个寄存器都是易失性的。
* 参见：https://masm32.com/board/index.php?topic=4702.0
*/

int sub_405ce3(
    struct patch_search_item* items,
    unsigned int mask,
    int clearLsbBit2
)
{
    int count = 0;
    
    assert(sizeof(struct patch_search_item) == 0x2c);
    for (struct patch_search_item* ecx_ptr = items; ecx_ptr->moduleFileName != (void*)-1; ecx_ptr++)
    {
        assert(offsetof(struct patch_search_item, v7) == 0x18 + 4);
        if (mask != (unsigned int)-1
            && !(ecx_ptr->v7 & mask))
        {
            continue;
        }

        assert(offsetof(struct patch_search_item, v6_zero) == 0x18);
        if (clearLsbBit2)
        {
            // 11111111111111111111111111111011
            ecx_ptr->v6_zero &= 0xfffffffb;
        }
        else
        {
            // 00000000000000000000000000000100
            ecx_ptr->v6_zero |= 4;
        }
        count++;
    }

    return count;
}