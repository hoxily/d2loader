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

    // ���д��볢�Ի�ȡ����ģ���������� d2loader.exe ʱ���滻�� game.exe ���������Ǿ�
    // ��֪���Լ���hook�ˡ�
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
    * �����jnb��ָjump short if not below (CF=0)
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
