#include "sub_405d90.h"
#include <stdio.h>
#include "sub_404ed0.h"

BOOL sub_405d90_ScriptLoadFile(
    const char* scriptFileName
)
{
    FILE* fp = fopen(scriptFileName, "r");
    if (fp == NULL)
    {
        sub_404ed0_LogFormat(
            LOG_TAG,
            "Error Loading Script File %s",
            scriptFileName
        );
        return FALSE;
    }
    
    sub_404ed0_LogFormat(
        LOG_TAG,
        "Sorry, ScriptLoadFile is not implemented yet!"
    );
    //TODO: 暂时用不上 d2hack.script。以后再实现。
    fclose(fp);
    return FALSE;
}