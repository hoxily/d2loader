#pragma once

/*
* ����ʮ�����Ʊ����ֽ����ݡ�
* ���磺B8xxxxxxxx C605xxxxxxxx01 E8xxxxxxxx 0FB6086A00890D �������޳��ո���Tab�ַ���
* ÿ����ASCII�ַ���Ϊһ���ֽڽ��н�����
* �����޷�������ʮ�������ֽڣ��洢Ϊ 0��
* ����ܽ�������ô�洢Ϊ 0xff00 | (byteValue & 0xff)��
* @param str ��ʮ�����Ʊ����ֽ�����
* @param buffer ��������洢��buffer��
* @returns �����ɹ����ֽ���
*/
extern int sub_405c59_ParseHexByteString(
    const char* str,
    int* buffer
);