#include <assert.h>
#include "sub_405ce3.h"

/*
* �������ʹ���� edx �Ĵ��������ǲ�û����һ�� push edx��popd edx ����������
* ���� Intel Application Binary Interface �淶��eax��ecx��edx ����Ĵ���������ʧ�Եġ�
* �μ���https://masm32.com/board/index.php?topic=4702.0
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