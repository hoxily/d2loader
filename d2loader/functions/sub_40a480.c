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

    // ������һƬ������ nop ָ�����ûɶ�ã��˴�������ԡ�

    // ����ǳ����⣬��ǰ������������ʹ�� ret ָ��ص��÷�������ʹ���� jmp ָ����ת��ȥ�ġ�
    // �ᵼ�� ida ��ջƽ�����������Ҫ�ֹ�������
    // �⻹�ᵼ��һ��������Ǿ��� sub_406d1e ����ֵΪ FALSE ʱ�����µ��÷��� false ��ִ֧�������飬
    // Ҳ����log��������� "Game Initialize Failed, Exitting\n"��
    // �����ʹ�� ida �Ķ�̬���Թ��ܣ��� eax ֵ�޸�Ϊ 0�����ܹ۲쵽��������
    return sub_406d1e_D2Init();
}
