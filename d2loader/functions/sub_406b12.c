#include <stddef.h>
#include <assert.h>
#include "sub_406b12.h"
#include "../global-variables.h"
#include "../constants.h"

BOOL sub_406b12_StoreCommandLineSetting(
    int i,
    char* buffer
)
{
    assert(offsetof(struct string_index_item, type) == 0x10);
    DWORD type = global_dd_402ea8_CommandLineArgumentTable[i].type;
    DWORD offset = global_dd_402ea8_CommandLineArgumentTable[i].offset;
    if (type == ARG_TYPE_NO_PARAM)
    {
        BYTE toggle = global_dd_408620_settings->padding[offset];
        // ԭ������ xor byte ptr [eax], 1
        // �������ᷭתԭ���Ѿ����úõ������
        // Ԥ�⣺d2loader.exe -w -w  ���������������������ᴰ�ڻ����У�����ȫ�����С�
        // ʵ������ȷʵ��û�д��ڻ����С�
        global_dd_408620_settings->padding[offset] = !toggle;
    }
    else if (type == ARG_TYPE_UNSIGNED_SHORT_INT)
    {
        /*
        * ��� base(_Radix) Ϊ0����ʹ�� strSource ָ����ַ����ĳ�ʼ�ַ���ȷ������
        * �����һ���ַ�Ϊ 0���ҵڶ����ַ���Ϊ��x����X�����򽫸��ַ�����Ϊ�˽���������
        * �����һ���ַ�Ϊ��0�����ҵڶ����ַ�Ϊ��x����X�����򽫸��ַ�����Ϊʮ������������
        * �����һ���ַ��ǡ�1������9�����򽫸��ַ�����Ϊʮ����������
        */
        WORD ax = (WORD)strtoul(buffer, NULL, 0);
        WORD* ptr = (WORD*)(global_dd_408620_settings->padding + offset);
        *ptr = ax;
    }
    else if (type == ARG_TYPE_UNSIGNED_INT)
    {
        DWORD eax = strtoul(buffer, NULL, 0);
        DWORD* ptr = (DWORD*)(global_dd_408620_settings->padding + offset);
        *ptr = eax;
    }
    else
    {
        // ��ʱ type ��ʾ�ַ����������Ĵ�С��
        int settingBufferSize = type;
        // ʹ�� (void) ���� lstrcpynA �ķ��ؽ��������Visual Studio����warning��
        (void)lstrcpynA(global_dd_408620_settings->padding + offset, buffer, settingBufferSize);
        // ǿ�����û����������һ���ַ�Ϊnull�����������������
        global_dd_408620_settings->padding[offset + settingBufferSize - 1] = '\0';
    }

    return TRUE;
}
