#pragma once

/*
* __func__ 会根据所在的函数，自动转为该函数名对应的字符串。
* 参见 https://docs.microsoft.com/en-us/cpp/preprocessor/predefined-macros?view=msvc-160#standard-predefined-identifier
*/
#define LOG_TAG __func__

extern void sub_404ed0_LogFormat(
    const char* tag,
    const char* format,
    ...
);
