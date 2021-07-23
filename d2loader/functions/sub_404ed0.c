#include "pch.h"
#include "sub_404ed0.h"
#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include "../constants.h"
#include "../global-variables.h"

void sub_404ed0_LogFormat(
    const char* tag,
    const char* format,
    ...
)
{
    time_t now;
    char timestamp[32];
    time(&now);
    struct tm* localNow = localtime(&now);
    if (localNow)
    {
        strftime(timestamp, sizeof(timestamp), "%b %d %H:%M:%S", localNow);
    }
    else
    {
        strcpy(timestamp, "?");
    }

    if (global_dd_408590_logFlag & LOG_TYPE_FILE)
    {
        if (global_dd_40858c_logFile)
        {
            fprintf(global_dd_40858c_logFile, "%s %s: ", timestamp, tag);

            va_list ap;
            va_start(ap, format);
            vfprintf(global_dd_40858c_logFile, format, ap);
            va_end(ap);

            fprintf(global_dd_40858c_logFile, "\n");
            fflush(global_dd_40858c_logFile);
        }
    }

    if (global_dd_408590_logFlag & LOG_TYPE_CONSOLE)
    {
        // ԭʼ������� _iob + 0x20 д����־��
        // ���µ�MSVCû�� _iob �ˡ��ɴ��Ϊ stdout �ɡ�
        FILE* consoleFilePtr = stdout;

        fprintf(consoleFilePtr, "%s %s: ", timestamp, tag);

        va_list ap;
        va_start(ap, format);
        vfprintf(consoleFilePtr, format, ap);
        va_end(ap);

        fprintf(consoleFilePtr, "\n");
        fflush(consoleFilePtr);
    }
}
