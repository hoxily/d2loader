#include "sub_40a480.h"
#include "sub_40a440.h"
#include "sub_4053b3.h"
#include "sub_40a9a0.h"

BOOL sub_40a480(
)
{
    // ��������Ĵ������֡�һ��Ƭ������ nop ָ�

    sub_40a440();
    if (sub_4053b3_IsWin32NtPlatform())
    {
        sub_40a9a0();
    }
    return FALSE;//TODO
}
