#pragma once

/* 根据函数标识符创建对应的字符串 */
#define LOG_TAG(identifier) #identifier

extern void sub_404ed0_LogFormat(
    const char* tag,
    const char* format,
    ...
);
