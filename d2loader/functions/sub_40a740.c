#include "pch.h"
#include "sub_40a740.h"
#include <assert.h>

BOOL sub_40a4e5_SearchInCommandLine(
    const char* arg
)
{
    char* commandLine = GetCommandLineA();
    char* buffer = malloc(strlen(commandLine) + 1);
    assert(buffer != NULL);
    strcpy(buffer, commandLine);

    char* eax_ptr = buffer;
    char* esi_ptr = buffer;
    BOOL find;
    while (TRUE)
    {
        assert(' ' == 0x20);
        if (*esi_ptr == ' ')
        {
            *esi_ptr = '\0';
            if (strcmp(eax_ptr, arg) == 0)
            {
                find = TRUE;
                break;
            }
            else
            {
                eax_ptr = esi_ptr + 1;
            }
        }
        else if (*esi_ptr == '\0')
        {
            if (strcmp(eax_ptr, arg) == 0)
            {
                find = TRUE;
                break;
            }
            else
            {
                find = FALSE;
                break;
            }
        }

        esi_ptr++;
    }
    free(buffer);

    return find;
}