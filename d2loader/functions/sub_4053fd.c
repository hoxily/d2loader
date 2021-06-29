#include "sub_4053fd.h"
#include "../global-variables.h"

HWND WINAPI sub_4053fd_my_FindWindowA(
    LPCSTR lpClassName,
    LPCSTR lpWindowName
)
{
    HWND hWindow;

    if (global_dd_4085a8_FindWindowA == NULL)
    {
        /*
        * 这里的代码好像有错误。
        *
        * xor eax, eax
        * call eax
        *
        * 这相当于调用地址为0处的函数。肯定是不行的。
        */

        // fix by hoxily@qq.com
        hWindow = FindWindowA(lpClassName, lpWindowName);
    }
    else
    {
        // 这里也很奇怪。没有像 sub_4054c2_my_LoadLibraryA, sub_40543b_my_GetModuleFileNameA
        // 那样调用缓存起来的函数针指。而是直接调用了导入的 FindWindowA
        hWindow = FindWindowA(lpClassName, lpWindowName);
    }

    if (hWindow == NULL)
    {
        return NULL;
    }

    // 当 ClassName 为 "Diablo II" 时返回 NULL。返回 NULL 不就相当于找不到吗？
    // 按理说，当前这个进程的窗口就是 "Diablo II" 吧？
    return lstrcmpiA("Diablo II", lpClassName) != 0 ? hWindow : NULL;
}
