#include "tests.h"
#include "constants.h"
#include "global-variables.h"
#include "functions/sub_407f21.h"
#include "functions/sub_404ed0.h"

void StringSplitsTest(
    const char* pattern,
    const char* separators
)
{
    sub_404ed0_LogFormat(
        LOG_TAG,
        "pattern string: \"%s\"",
        pattern
    );
    sub_404ed0_LogFormat(
        LOG_TAG,
        "separators string: \"%s\"",
        separators
    );
    DWORD count;
    char** splits = sub_407f21_SplitString(
        pattern,
        separators,
        &count
    );

    if (splits == NULL)
    {
        sub_404ed0_LogFormat(
            LOG_TAG,
            "split string failed."
        );
        return;
    }

    sub_404ed0_LogFormat(
        LOG_TAG,
        "split string succeeded. count = %d",
        count
    );

    for (char** ptr = splits, i = 0; *ptr != NULL; ptr++, i++)
    {
        sub_404ed0_LogFormat(
            LOG_TAG,
            "[%d]: \"%s\"",
            i,
            *ptr
        );
    }

    free(splits);
    splits = NULL;
    sub_404ed0_LogFormat(
        LOG_TAG,
        ""
    );
}

void InitConsoleOutput(
)
{
    global_dd_408590_logFlag = LOG_TYPE_CONSOLE;
    // Win32窗口程序使用控制台，参见：https://blog.csdn.net/qq_31042143/article/details/107307860
    AllocConsole();
    (void)freopen("conout$", "w", stdout);
    (void)freopen("conout$", "w", stderr);
}

void Tests(
)
{
    InitConsoleOutput();

    StringSplitsTest(
        "7, abc,,,,,,xyz,",
        ","
    );
    StringSplitsTest(
        "7, abc,,,,,,xyz,",
        "7, abcxyz"
    );
    StringSplitsTest(
        "7, abc,,,,,,xyz,",
        ", "
    );
}