#include <Windows.h>
#include <assert.h>
#include <stdio.h>
#include "logger.h"
#include "constant-strings.h"
#include "constants.h"
#include "data-types.h"
#include "global-variables.h"
#include "functions/sub_404ed0.h"
#include "functions/sub_4053b3.h"
#include "functions/sub_40532e.h"



BOOL sub_40513a(
    HMODULE hModule,
    const char* hookDll,
    struct hook_search_item* functionNameList,
    void* null1,
    void* null2)
{
    if (hookDll == NULL)
    {
        SetLastErrorEx(ERROR_INVALID_PARAMETER, 1);
        return FALSE;
    }

    if (functionNameList == NULL)
    {
        SetLastErrorEx(ERROR_INVALID_PARAMETER, 1);
        return FALSE;
    }

    DWORD functionNameListCount = 0;
    for (struct hook_search_item* eax_item = functionNameList; eax_item->functionName != (const char*)-1; eax_item++)
    {
        functionNameListCount++;
    }

    if (functionNameListCount == 0)
    {
        SetLastErrorEx(ERROR_INVALID_PARAMETER, 1);
        return FALSE;
    }

    if (null1 != NULL)
    {
        if (IsBadWritePtr(null1, functionNameListCount * 4))
        {
            SetLastErrorEx(ERROR_INVALID_PARAMETER, 1);
            return FALSE;
        }
    }

    if (null2 != NULL)
    {
        if (IsBadWritePtr(null2, 4))
        {
            SetLastErrorEx(ERROR_INVALID_PARAMETER, 1);
            return FALSE;
        }
    }

    if (functionNameListCount != 0)
    {
        DWORD ebx_count = 0;
        struct hook_search_item* esi_item = functionNameList;
        do
        {
            if (esi_item->functionName == NULL)
            {
                return FALSE;
            }
            void* ptr = esi_item->functionProcAddress;
            if (ptr != NULL)
            {
                if (IsBadCodePtr(ptr))
                {
                    return FALSE;
                }
            }

            ebx_count++;
            esi_item++;
        } while (ebx_count < functionNameListCount);
    }

    BOOL isWin32NtPlatform = sub_4053b3_IsWin32NtPlatform();
    if (!isWin32NtPlatform)
    {
        if ((void*)hModule >= (void*)0x80000000)
        {
            SetLastErrorEx(ERROR_INVALID_HANDLE, 1);
            return FALSE;
        }
    }

    if (null1 != NULL)
    {
        memset(null1, 0, functionNameListCount * 4);
    }

    if (null2 != NULL)
    {
        memset(null2, 0, 4);
    }

    IMAGE_IMPORT_DESCRIPTOR* importDescriptor = sub_40532e(hModule, hookDll);
    if (importDescriptor == NULL)
    {
        return FALSE;
    }
    
    assert(offsetof(IMAGE_IMPORT_DESCRIPTOR, OriginalFirstThunk) == 0);
    assert(offsetof(IMAGE_IMPORT_DESCRIPTOR, FirstThunk) == 0x10);
    IMAGE_THUNK_DATA32* edi_originalThunkData = (IMAGE_THUNK_DATA32*)((char*)hModule + importDescriptor->OriginalFirstThunk);
    IMAGE_THUNK_DATA32* esi_thunkData = (IMAGE_THUNK_DATA32*)((char*)hModule + importDescriptor->FirstThunk);
    
    assert(sizeof(IMAGE_THUNK_DATA32) == sizeof(DWORD));
    // fix by hoxily@qq.com, 下面这个循环里已经不再使用 hookDll 参数的值，把它当做一个DWORD大小的变量来用。
    // 为了方便，这里我们额外定义一个 isByOrdinal 变量。
    BOOL isByOrdinal;
    IMAGE_IMPORT_BY_NAME* var_c = NULL;
    while (*(DWORD*)edi_originalThunkData != 0)
    {
        if (IMAGE_SNAP_BY_ORDINAL32(edi_originalThunkData->u1.Ordinal))
        {
            isByOrdinal = TRUE;
        }
        else
        {
            isByOrdinal = FALSE;
            var_c = (IMAGE_IMPORT_BY_NAME*)((char*)hModule + edi_originalThunkData->u1.AddressOfData);
            if (var_c->Name[0] == '\0')
            {
                // fix by hoxily@qq.com, 这里没有对 edi 做增长，就跳去循环入口。
                // 一旦遇到一个非Ordinal，但是Name数据又是空的项时，将会导致死循环。
                edi_originalThunkData++;
                // esi 也需要增长。
                esi_thunkData++;
                continue;
            }
        }

        if (functionNameListCount <= 0)
        {
            edi_originalThunkData++;
            esi_thunkData++;
            continue;
        }

        struct hook_search_item* eax_item = functionNameList;
        DWORD ebx_i = 0;
        BOOL hookFlag = FALSE;
        for (; ebx_i < functionNameListCount; ebx_i++)
        {
            if (!isByOrdinal)
            {
                if (((DWORD)eax_item->functionName & 0x80000000) == 0)
                {
                    if (lstrcmpiA(eax_item->functionName, var_c->Name) == 0)
                    {
                        if (functionNameList[ebx_i].functionProcAddress == NULL)
                        {
                            break;
                        }
                        else
                        {
                            hookFlag = TRUE;
                            break;
                        }
                    }
                }
            }
            else
            {
                if (edi_originalThunkData->u1.Ordinal == (DWORD)eax_item->functionName)
                {
                    hookFlag = TRUE;
                    break;
                }
            }
            eax_item++;
        }

        if (hookFlag)
        {
            MEMORY_BASIC_INFORMATION memoryBasicInfo;
            DWORD oldProtect;
            assert(sizeof(memoryBasicInfo) == 0x1c);
            VirtualQuery(
                esi_thunkData,
                &memoryBasicInfo,
                sizeof(memoryBasicInfo)
            );
            VirtualProtect(
                memoryBasicInfo.BaseAddress,
                memoryBasicInfo.RegionSize,
                PAGE_EXECUTE_READWRITE,
                &memoryBasicInfo.Protect
            );
            if (null1 != NULL)
            {
                DWORD* ptr = (DWORD*)null1;
                ptr[ebx_i] = esi_thunkData->u1.Function;
            }

            esi_thunkData->u1.Function = (DWORD)functionNameList[ebx_i].functionProcAddress;
            VirtualProtect(
                memoryBasicInfo.BaseAddress,
                memoryBasicInfo.RegionSize,
                memoryBasicInfo.Protect,
                &oldProtect
            );
            if (null2 != NULL)
            {
                DWORD* ptr = (DWORD*)null2;
                (*ptr)++;
            }
        }

        edi_originalThunkData++;
        esi_thunkData++;
    }

    SetLastError(ERROR_SUCCESS);
    return TRUE;
}

BOOL sub_4054fd_HookDll()
{
    HMODULE hModuleKernel32 = GetModuleHandleA("Kernel32.dll");
    assert(hModuleKernel32 != NULL);
    global_dd_4085a0_LoadLibraryA = (fn_LoadLibraryA)GetProcAddress(hModuleKernel32, "LoadLibraryA");
    global_dd_4085a4_GetModuleFileNameA = (fn_GetModuleFileNameA)GetProcAddress(hModuleKernel32, "GetModuleFileNameA");
    
    HMODULE hModuleUser32 = GetModuleHandleA("User32.dll");
    assert(hModuleUser32 != NULL);
    global_dd_4085a8_FindWindowA = (fn_FindWindowA)GetProcAddress(hModuleUser32, "FindWindowA");
    
    char filename[0x104];
    GetModuleFileNameA(NULL, filename, sizeof(filename));
    global_dd_408598_moduleD2LoaderFileName = _strdup(filename);

    char* filePart;
    GetFullPathNameA("game.exe", sizeof(filename), filename, &filePart);
    global_dd_40859c_gameDotExeFileName = _strdup(filename);

    /*
    * 这儿有段代码比较难理解。
    * mov [esp + 118h + lpLibFileName], offset LibFileName ; "bnclient.dll"
    * 
    * 其中的 lpLibFileName 其实就是 -118h，相当于把当前栈顶元素赋予这个字符串常量首地址。
    * 
    * 最神奇的是在这之前调用了 __cdecl 规范的 _strdup 函数，还没有平衡栈。
    * 也就相当于 pop ecx; mov ecx, offset LibFileName; push ecx;
    */
    global_dd_4085b0_moduleBnClient = LoadLibraryA("bnclient.dll");
    global_dd_4085ac_moduleD2Gfx = LoadLibraryA("d2gfx.dll");
    global_dd_4085b4_moduleD2Net = LoadLibraryA("d2net.dll");

    BOOL ret1 = sub_40513a(
        global_dd_4085b0_moduleBnClient,
        "Kernel32.dll",
        &global_dd_4023f0[0],
        NULL,
        NULL
    );
    BOOL ret2 = sub_40513a(
        global_dd_4085ac_moduleD2Gfx,
        "User32.dll",
        &global_dd_402408[0],
        NULL,
        NULL
    );
    
    return ret1 && ret2;
}

BOOL sub_40a480()
{
    //TODO
}

void sub_4057a8()
{
    //TODO
}

void sub_40a600_Cleanup()
{
    //TODO
}

void sub_4071a5()
{
    //TODO
}

void sub_405af1()
{
    //TODO
}

void sub_406175()
{
    //TODO
}

void sub_405602()
{
    //TODO
}

void sub_4069bc()
{
    //TODO
}

void sub_404d77_GameClientLoop()
{
    //TODO
}

#include "functions/sub_404b60.h"

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd)
{
    InitLogFile();

    return sub_404b60_WinMain(
        hInstance,
        hPrevInstance,
        lpCmdLine,
        nShowCmd
    );
}
