#include "sub_406d1e.h"
#include "sub_4070d5.h"
#include "sub_404ed0.h"

BOOL sub_406d1e_D2Init(
)
{
    if (!sub_4070d5_D2CommonInit())
    {
        /*
        * ��ǰ�������� MessageBoxA ʱѹջ�Ĳ����Ƚ���֡���ѹ��ջ�У�Ȼ���¼���Ĵ������ٴ�ѹջ��
        * ��IDA����ظ��ú�������������ֲ�������lpText��lpCaption��
        * ���Ե����Ҽ��˵��еġ�Edit function...�����ڵ�����Edit function�Ի����н� 
        * Local variables area ����Ϊ��ȷ�Ĵ�С��
        * 
        * push offset ��������
        * push offset ��������
        * push 10h ; �� MB_ICONERROR
        * mov eax, [esp+8] ; eax ָ��ո�push��������������
        * mov ebx, [esp+4] ; ebx ָ��ո�push�����ı�������
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
    return FALSE;//TODO
}
