#include "sub_406ac0.h"
#include "../global-variables.h"

int sub_406ac0_FindArgumentNameIndex(
    const char* buffer
)
{
    if (buffer == 0) {
        return -1;
    }

    if (buffer[0] != '-')
    {
        return -1;
    }

    int ebp_index = 0;
    if (global_dd_402ea8_CommandLineArgumentTable[0].category == (void*)ebp_index)
    {
        return -1;
    }

    // 指向去除“-”的参数名
    const char* ebx_argName = buffer + 1;

    struct string_index_item* esi_stringTableEntry = &global_dd_402ea8_CommandLineArgumentTable[0];
    do
    {
        const char* shortName = esi_stringTableEntry[0].shortName;
        if (lstrcmpiA(shortName, ebx_argName) == 0)
        {
            break;
        }
        const char* longName = esi_stringTableEntry[0].longName;
        if (lstrcmpiA(longName, ebx_argName) == 0)
        {
            break;
        }
        esi_stringTableEntry++;
        ebp_index++;
    } while (esi_stringTableEntry[0].category != NULL);

    // hoxily fix: 如果没有找到合适的参数名，则返回 -1。
    if (esi_stringTableEntry[0].category == NULL)
    {
        return -1;
    }
    else
    {
        return ebp_index;
    }
}
