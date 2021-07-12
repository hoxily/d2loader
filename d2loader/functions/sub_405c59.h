#pragma once

/*
* 解析十六进制表达的字节数据。
* 例如：B8xxxxxxxx C605xxxxxxxx01 E8xxxxxxxx 0FB6086A00890D 将会在剔除空格与Tab字符后，
* 每两个ASCII字符作为一个字节进行解析。
* 对于无法解析的十六进制字节，存储为 0；
* 如果能解析，那么存储为 0xff00 | (byteValue & 0xff)；
* @param str 以十六进制表达的字节数据
* @param buffer 解析结果存储到buffer中
* @returns 解析成功的字节数
*/
extern int sub_405c59_ParseHexByteString(
    const char* str,
    int* buffer
);