#include "logger.h"
#include <stdio.h>
#include <stdarg.h>

static FILE* logFile;

void InitLogFile()
{
    logFile = fopen("d2loader1.log", "a");
}

void LogFormat(const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    vfprintf(logFile, format, ap);
    va_end(ap);
}