#pragma once

/*
* __func__ ��������ڵĺ������Զ�תΪ�ú�������Ӧ���ַ�����
* �μ� https://docs.microsoft.com/en-us/cpp/preprocessor/predefined-macros?view=msvc-160#standard-predefined-identifier
*/
#define LOG_TAG __func__

extern void sub_404ed0_LogFormat(
    const char* tag,
    const char* format,
    ...
);
