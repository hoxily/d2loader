#include "sub_40543b.h"
#include "../global-variables.h"

DWORD WINAPI sub_40543b_my_GetModuleFileNameA(
    HMODULE hModule,
    LPSTR lpFilename,
    DWORD nSize
)
{
    DWORD len;
    if (global_dd_4085a4_GetModuleFileNameA == NULL)
    {
        len = GetModuleFileNameA(hModule, lpFilename, nSize);
    }
    else
    {
        len = global_dd_4085a4_GetModuleFileNameA(hModule, lpFilename, nSize);
    }

    if (global_dd_408598_moduleD2LoaderFileName == NULL)
    {
        return len;
    }

    if (global_dd_40859c_gameDotExeFileName == NULL)
    {
        return len;
    }

    if (len == 0)
    {
        return len;
    }

    // 当有代码尝试获取到的模块名正好是 d2loader.exe 时，替换成 game.exe ，这样它们就
    // 不知道自己被hook了。
    if (lstrcmpiA(lpFilename, global_dd_408598_moduleD2LoaderFileName) != 0)
    {
        return len;
    }

    (void)lstrcpynA(lpFilename, global_dd_40859c_gameDotExeFileName, nSize);
    lpFilename[nSize - 1] = '\0';
    /*
    * call esi ; esi is lstrlenA
    * cmp eax, edi ; edi is nSize - 1
    * jnb short loc_4054b7
    *
    * 这里的jnb是指jump short if not below (CF=0)
    */
    if ((DWORD)lstrlenA(global_dd_40859c_gameDotExeFileName) >= nSize - 1)
    {
        return nSize - 1;
    }
    else
    {
        return lstrlenA(global_dd_40859c_gameDotExeFileName);
    }
}
