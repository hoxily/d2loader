#include "sub_40a480.h"
#include "sub_40a440.h"
#include "sub_4053b3.h"
#include "sub_40a9a0.h"
#include "sub_406d1e.h"

BOOL sub_40a480(
)
{
    sub_40a440();
    if (sub_4053b3_IsWin32NtPlatform())
    {
        sub_40a9a0();
    }

    // 这里有一片连续的 nop 指令。由于没啥用，此处将其忽略。

    // 这里非常特殊，当前函数不是正常使用 ret 指令返回调用方，而是使用了 jmp 指令跳转回去的。
    // 会导致 ida 的栈平衡分析出错，需要手工修正。
    // 这还会导致一个结果，那就是 sub_406d1e 返回值为 FALSE 时，导致调用方的 false 分支执行了两遍，
    // 也就是log输出了两遍 "Game Initialize Failed, Exitting\n"。
    // 你可以使用 ida 的动态调试功能，将 eax 值修改为 0，就能观察到这种现象。
    return sub_406d1e_D2Init();
}
