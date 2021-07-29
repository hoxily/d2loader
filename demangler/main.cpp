#include <Windows.h>
// 需要在包含 DbgHelp.h 之前先包含 Windows.h ,否则编译报错。
#include <DbgHelp.h>
#include <cstdio>
#include <cstring>

char global_mangledName[1024];
char global_demangledName[2048];

// UnDecorateSymbolName 函数的说明参见：https://docs.microsoft.com/en-us/windows/win32/api/dbghelp/nf-dbghelp-undecoratesymbolname

/*
* 输入C++ mangled名字，输出修饰之前的名字。
* 输入EOF(Ctrl-Z)退出程序。
*/
int main()
{
    while (true)
    {
        printf("input mangled name: ");
        global_mangledName[0] = '\0';
        fgets(global_mangledName, sizeof(global_mangledName), stdin);
        size_t len = strlen(global_mangledName);
        if (len <= 0)
        {
            break;
        }
        if (global_mangledName[len - 1] == '\n')
        {
            global_mangledName[len - 1] = '\0';
        }

        DWORD ret = UnDecorateSymbolName(
            global_mangledName,
            global_demangledName,
            sizeof(global_demangledName),
            UNDNAME_COMPLETE
        );

        if (ret > 0)
        {
            printf("demangled name is: %s\n", global_demangledName);
        }
        else
        {
            printf("demangle failed. Error code: 0x%08x\n", GetLastError());
        }
    }
    
    return 0;
}