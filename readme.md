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
可以借助汇编中的寄存器名字，作为局部变量的前缀，以方便对照汇编代码。例如：

```c
int eax_i = 0;
```

## 遇到的困难

### 三元运算符的无跳转优化

sub_40532e 函数的结尾处，有一个三元运算符。c语言代码如下：
```c
return importDescriptor->Name ? importDescriptor : NULL;
```
其对应的反汇编代码如下：
```asm
mov eax, [esi + 0ch] ;esi 当前为 importDescriptor 指针
neg eax
sbb eax, eax
and eax, esi
...
retn
```
neg、sbb组合可以将eax变为0或者0xffffffff。
最后 and 运算，就相当于根据 importDescriptor->Name 是否为0，来决定要返回 importDescriptor还是NULL。

### 函数实参变量另作它用

在 sub_40513a 函数末尾的while循环里，反汇编代码把函数实参 `const char* hookDll` 变量用于表达导入函数列表项是不是以Ordinal方式导入。而不是重新定义一个局部变量。

这可能是因为hookDll变量的参数值在这个循环以及后面的代码里用不上了。

### 内联函数无ret直接jmp回去

函数 sub_40a480 不是以正常的ret指令返回调用方，而是以jmp指令返回调用方。
把代码简化一下，就是如下的形式：

```asm
functionA:
  call functionB
onBeforeTest:
  test eax, eax
  jnz onOk
  ...; log 输出错误信息
  xor eax, eax
  retn
onOk:
  ...; functionB返回非零时的处理
  retn

functionB:
  pusha
  ...; 执行一些操作
  popa
  jmp onBeforeTest
```
这会导致 ida 的栈平衡分析出错，需要手工修正。这还会导致一个结果，那就是 sub_406d1e 返回值为 FALSE 时，导致调用方的 false 分支执行了两遍，也就是log输出了两遍 "Game Initialize Failed, Exitting\n"。

你可以使用 ida 的动态调试功能，将 eax 值修改为 0，就能观察到这种现象。

### 缓冲区首地址+1被识别为另一个变量

见 sub_4068f2_LoadConfFile 函数的反编译结果。
```
Buf = byte ptr - 2800h
var_27FF = byte ptr -27FFh
```
其实这个函数只定义了一个 `char buffer[0x2800];` 的缓冲区，出现 var_27FF 变量只是因为在代码里做了如下的操作。
```c
buffer[eax_i + 1] = '\0';
```

## 编码技巧

### 巧用 union 联合体做结构体成员的偏移

由于在汇编中，结构体的成员的偏移量是以立即数或寻址中的常量出现的，不像C语言中可以使用命名的成员来访问。

并且在汇编代码中，对结构体的成员访问是随机的，并不是从第一个成员开始，顺序访问下来。用到哪个成员，就访问那个成员。

所以问题就变成，汇编代码中出现了哪个成员，我们就需要在C语言中构造出这个成员来。但是这个成员的偏移量是由该成员之前的成员决定的。这就很麻烦。

于是有了如下的联合体内嵌套结构体的构造，摘录自 `data-types.h` 文件：

```c
#pragma pack(1) // 逐字节对齐，方便定位
union program_setting_store
{
    #pragma pack(1)
    struct
    {
        // 不允许使用不完整的类型
        // BYTE offset[0];
        BYTE value;
    } db_0000_expansion;

    #pragma pack(1)
    struct
    {
        BYTE offset[4];
        char value[5 + 1];
    } db_0004_str;

    // 最后定义一个 padding 字节数组，确保该联合体的大小为指定的大小。
    BYTE padding[0xc94];
};
```
如上所示，在嵌套的内层struct中，先定义一个offset成员，让其占据指定的偏移量。然后再定义所需的数据变量。简直妙不可言！


利用log中的tag标签为函数取名。TODO

assert断言确保结构体大小符合要求。TODO

offsetof求取结构体成员的偏移量。TODO

## 参考文档

ida sp-analysis failed:TODO

msdn win32 api:TODO
