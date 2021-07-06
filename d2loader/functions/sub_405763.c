#include <assert.h>
#include "sub_405763.h"
#include "../data-types.h"
#include "../global-variables.h"

BOOL sub_405763_InitD2FunctionTable(
)
{
    struct d2_dll_function_item* esi_ptr = global_dd_402750_functionTable;
    if (esi_ptr == NULL)
    {
        return FALSE;
    }

    while (TRUE)
    {
        assert(offsetof(struct d2_dll_function_item, module) == 0);
        assert(offsetof(struct d2_dll_function_item, procedure) == 4);
        if (esi_ptr->module == NULL || esi_ptr->procedure == NULL)
        {
            return TRUE;
        }

        assert(offsetof(struct d2_dll_function_item, ordinal) == 8);
        esi_ptr->procedure = GetProcAddress(*esi_ptr->module, (LPCSTR)esi_ptr->ordinal);
        if (esi_ptr->procedure == NULL)
        {
            return FALSE;
        }
        assert(sizeof(struct d2_dll_function_item) == 0xc);
        esi_ptr++;
    }
    
    return TRUE;
}
