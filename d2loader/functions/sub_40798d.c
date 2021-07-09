#include "sub_40798d.h"
#include "sub_407778.h"

void* sub_40798d_DuplicateMemoryBlock(
    void* src,
    size_t size
)
{
    void* esi_ptr = malloc(size);
    if (esi_ptr == NULL)
    {
        return NULL;
    }

    sub_407778_smemcpy(esi_ptr, src, size);
    
    return esi_ptr;
}