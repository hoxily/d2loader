#include "sub_4066dc.h"
#include "../global-variables.h"

BOOL sub_4066dc_PrintParametersTable(
    FILE* fp
)
{
    if (fp == NULL)
    {
        return FALSE;
    }

    struct string_index_item* entry = &global_dd_402ea8_CommandLineArgumentTable[0];
    if (entry->category != NULL)
    {
        do
        {
            //esi Ö¸Ïò entry->longName
            fprintf(fp, "%-12s %-12s 0x%08X\n", entry->category, entry->longName, entry->offset);
            entry++;
        } while (entry->category != NULL);
    }

    fflush(fp);
    return TRUE;
}
