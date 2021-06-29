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
        * ����Ĵ�������д���
        *
        * xor eax, eax
        * call eax
        *
        * ���൱�ڵ��õ�ַΪ0���ĺ������϶��ǲ��еġ�
        */

        // fix by hoxily@qq.com
        hWindow = FindWindowA(lpClassName, lpWindowName);
    }
    else
    {
        // ����Ҳ����֡�û���� sub_4054c2_my_LoadLibraryA, sub_40543b_my_GetModuleFileNameA
        // �������û��������ĺ�����ָ������ֱ�ӵ����˵���� FindWindowA
        hWindow = FindWindowA(lpClassName, lpWindowName);
    }

    if (hWindow == NULL)
    {
        return NULL;
    }

    // �� ClassName Ϊ "Diablo II" ʱ���� NULL������ NULL �����൱���Ҳ�����
    // ����˵����ǰ������̵Ĵ��ھ��� "Diablo II" �ɣ�
    return lstrcmpiA("Diablo II", lpClassName) != 0 ? hWindow : NULL;
}
