#include "logger.h"
#include <stdio.h>
#include <stdarg.h>

static FILE* logFile;

void InitLogFile()
{
    fopen_s(&logFile, "d2loader.log", "a");
}

void LogFormat(const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    vfprintf(logFile, format, ap);
    va_end(ap);
}