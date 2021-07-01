## 反编译目标文件

| 属性名 | 属性值 |
| - | - |
| 文件名 | D2Loader.exe |
| MD5 | 270B200034F60488E7B1E2A5EAC2085D |
| SHA1 | 797AF89EB546656FE875794D4F703D6E03374813 |
| SHA256 | 69BD9820AF5E494E7C4C0FA8B10FE0FF2784E0EA2BA9358499EB3CB1678F8F07 |
| 大小 | 38,912 字节 |

## 反编译工具介绍

IDA Freeware windows 版本，下载地址：https://hex-rays.com/ida-free/

## IDA 使用技巧

全局变量重命名；TODO

局部变量重命名；TODO

反汇编中添加注释；TODO

手工调整call指令的栈平衡；TODO

动态调试；TODO

## 编码规范

### 全局变量

定义在 `global-variables.c` 文件中，其对应的声明则在 `global-variables.h` 文件中。

其格式为 `<变量类型> global_<数据类型前缀>_<十六进制地址>[_<附加名字>]` 。

`数据类型前缀` 用于标识该变量的字节大小，暂时有下面这几种类型。对于结构体、联合体、数组、指针等类型不是太适用，有待改善。

`附加名字` 属于可选项。用于在推测出该变量的用途后的有意义名字。使用 camelCase。

```
db: 1 字节
dw: 2 字节
dd: 4 字节
```

例子：
```c
FILE* global_dd_40858c_logFile;
```

### 函数名称

格式为 `<返回值类型> sub_<十六进制地址>[_<附加名字>]` 。

`附加名字` 属于可选项。用于在推测出该函数的用途后的有意义名字。使用 PascalCase。

为了简单起见，每个被IDA识别出来的函数都独立存放于一个以该函数基础名字命名的源文件中。

例子：
```
BOOL sub_407380_CheckFileExist(
    const char* filePath
)
{
    ...(函数体)
}
```
这个 `sub_407380_CheckFileExist` 的基础名字为 `sub_407380`，函数定义存放于 `functions/sub_407380.c` 文件中，其声明文件则位于 `functions/sub_407380.h` 文件中。

由于函数的参数列表有可能会很长，所以将每一个参数都单独放到一行，如上面例子所示。

对函数的文档注释放到声明文件中，而不是函数的定义文件中。

### 局部变量

尽量使用有意义的名字。或者直接沿用ida给出的名字。

## 遇到的困难

### 三元运算符的无跳转优化
TODO
### 函数实参变量另作它用
TODO
### 内联函数无ret直接jmp回去
TODO
### 缓冲区长度减1被识别为另一个变量
TODO
## 编码技巧

巧用 union 联合体做结构体偏移。TODO

利用log中的tag标签为函数取名。TODO

assert断言确保结构体大小符合要求。TODO

offsetof求取结构体成员的偏移量。TODO

## 参考文档

ida sp-analysis failed:TODO

msdn win32 api:TODO
