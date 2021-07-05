#include "sub_406d1e.h"
#include "../global-variables.h"
#include "sub_4070d5.h"
#include "sub_404ed0.h"

BOOL sub_406d1e_D2Init(
)
{
    if (!sub_4070d5_D2CommonInit())
    {
        /*
        * 当前函数调用 MessageBoxA 时压栈的操作比较奇怪。先压入栈中，然后记录到寄存器，再次压栈。
        * 让IDA错误地给该函数添加了两个局部变量：lpText和lpCaption。
        * 可以单击右键菜单中的“Edit function...”，在弹出的Edit function对话框中将 
        * Local variables area 调整为正确的大小。
        * 
        * push offset 内容文字
        * push offset 标题文字
        * push 10h ; 即 MB_ICONERROR
        * mov eax, [esp+8] ; eax 指向刚刚push进来的内容文字
        * mov ebx, [esp+4] ; ebx 指向刚刚push进来的标题文字
        * push ebx
        * push eax
        * push 0
        * call ds:MessageBoxA
        */
         
        MessageBoxA(
            NULL,
            "Common Init Failed",
            LOG_TAG,
            MB_ICONERROR
        );
        return FALSE;
    }

    if (global_dd_408620_settings->db_0988_extraMpq.value[0] != '\0')
    {
        BOOL success = dll_storm_266(
            &global_dd_408620_settings->db_0988_extraMpq.value[0],
            0x10000,
            2,
            &global_dd_408638
        );
        //TODO
    }
    return FALSE;//TODO
}
