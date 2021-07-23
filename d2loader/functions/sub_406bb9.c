#include "pch.h"
#include "sub_406bb9.h"
#include "../global-variables.h"

BOOL sub_406bb9_LoadVideoConfigFromRegistry(
)
{
    DWORD data;
    HKEY phkResult;
    DWORD cbData;
    DWORD type;

    LSTATUS ret = RegOpenKeyExA(
        HKEY_CURRENT_USER,
        "Software\\Blizzard Entertainment\\Diablo II\\VideoConfig",
        0,
        KEY_READ,
        &phkResult);

    if (ret != ERROR_SUCCESS)
    {
        return FALSE;
    }

    // Render ��һ��REG_DWORD���͵����ݡ������ڳɹ�ִ�� D2VidTst.exe ֮��ʹ��windows��ע���༭��������
    cbData = sizeof(data);
    ret = RegQueryValueExA(
        phkResult,
        "Render",
        NULL,
        &type,
        (LPBYTE)&data,
        &cbData);
    if (ret != ERROR_SUCCESS)
    {
        RegCloseKey(phkResult);
        return FALSE;
    }
    else
    {
        if (data == 1)
        {
            global_dd_408620_settings->db_0008_rave.value = TRUE;
        }
        else if (data == 2)
        {
            global_dd_408620_settings->db_0006_glide.value = TRUE;
        }
        else if (data == 3)
        {
            global_dd_408620_settings->db_0005.value = TRUE;
        }
        else if (data == 4)
        {
            global_dd_408620_settings->db_0004_window.value = TRUE;
        }
    }
    RegCloseKey(phkResult);
    return TRUE;
}
