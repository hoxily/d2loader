#include "sub_40798d.h"
#include "sub_407778.h"

void* sub_40798d(
    void* arg0,
    size_t size
)
{
    void* esi_ptr = malloc(size);
    if (esi_ptr == NULL)
    {
        return NULL;
    }

    sub_407778_smemcpy(esi_ptr, arg0, size);
    
    return esi_ptr;
}