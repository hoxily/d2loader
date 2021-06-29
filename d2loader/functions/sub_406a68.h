#pragma once

/**
 * 切出 args 中的第一个命令行参数，存入 buffer 中。
 * @param args 非空格开头的命令行参数字符串
 * @param buffer 存储结果的缓冲区
 * @returns 返回下一次处理时的起始位置
 * */
extern const char* sub_406a68_CutFirstArgument(
    const char* args,
    char* buffer
);
