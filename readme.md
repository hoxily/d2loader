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

IDA会将exe、dll反编译的结果作为一个i64工程进行保存。里面除了反汇编结果，还记录了我们在IDA上做的注释、重命名等操作，方便我们持续分析一个较大的程序。

### 全局变量重命名

当我们根据变量的用法，得到了一个表达其含义的附加名字时，我们可以不仅可以重命名C代码中的变量名，还可以重命名IDA中的变量名，以使两者保持一致。

具体的操作如下：

1. 单击Graph View中的全局变量名，它会变成黄底高亮的状态；
2. 单击鼠标右键弹出菜单，选择Rename菜单项；
3. 在弹出的Rename address对话框中，可以给IDA自动生成的变量名做修改。

### 局部变量重命名

与重命名全局变量同理，我们也可以重命名函数的局部变量名。

具体操作如下：

1. 单击Graph View中的局部变量名，它会变成黄底高亮的状态；
2. 单击鼠标右键弹出菜单，选择Rename菜单项；
3. 在弹出的Please enter a string对话框中，给IDA自动生成的变量名做修改。

### 反汇编中添加注释

IDA默认会给那些识别出来的windows api函数调用处添加注释，标明该参数的名字；或者当某个寄存器指向一个api函数时，后面的 call 指令会添加注释，标明调用的是哪个api函数。

有时候，我们可能想要额外添加一些注释，以解释某断特殊的代码。

具体操作如下：

1. 鼠标单击需要添加注释的行；
2. 按下键盘上的分号按键(;)来添加、修改一个repeatable注释；
3. 按下键盘上的冒号按键(:)来添加、修改一个常规的注释；

repeatable注释的特殊性在于，它能在其他引用了此处对象的地方展示出该注释。具体参见官方解释：Create a repeatable comment: https://hex-rays.com/products/ida/support/idadoc/480.shtml

### 手工调整call指令的栈平衡

当IDA给出的反编译结果，函数的结尾处出现标红的 `sp-analysis failed` 提示时，说明当前的栈已经不平衡了。

比如下面提到的 `内联函数无ret直接jmp回去` 小节，存在一种栈不平衡的情况。

什么是栈平衡呢？就是函数调用前后，栈指针恢复到调用前的状态。这涉及到调用约定(calling conversion)。

- `__cdecl` 调用约定是指函数参数从右到左的顺序依次push到栈上，调用方(caller)清理push进去的数据，恢复栈平衡。这可以实现可变参数数量的效果，例如printf函数。
- `__stdcall` 调用约定是指函数参数从右到左的顺序依次push到栈上，被调用方(callee)清理push进去的数据，恢得栈平衡。

首先我们需要打开栈指针指示，在 Options->Disassembly->Stack pointer 单选框上打勾，即可在每一行汇编代码的前面看到栈指针指示。

栈指针指示是一个16进制的数值。在函数刚开始时为0，每次push则会增长相应的量，每次pop则会减少相应的量。正常情况下，函数ret时其栈指针指示为0，这样才能正常使用栈上记录的返回地址返回。

当一个call指令的栈指针指示不正常时，可以如下操作：

1. 单击定位到该call指令所在行；
2. 按下Alt+K组合键，弹出Change SP value对话框；
3. 填写你觉得正确的变化量（可以是正的值，也可以是负的值）；
4. 观察函数返回处的栈指针指示是否已经变为正常的0；

### 动态调试

动态调试类似于Visual Studio调试C/C++程序，可以在汇编代码上设定断点，运行到断点即停下，以供查看当前状态。

对于静态分析难以判断的情况，可以使用动态调试来辅助。

执行程序的命令行参数可以在 Debugger->Process options 弹出的 Debug application setup: win32 对话框上进行设置。

有些程序需要管理员权限才能启动，这时需要以管理员权限启动IDA程序，就能调试了。

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

### 利用log中的tag标签为函数取名

原始的 d2loader.exe 代码中有许多log输出语句。并且log的格式为 `<日期> <时刻> <函数名> <日志内容>`。其中的`日期`与`时刻`是程序根据执行时的时间自动生成的，而`函数名`与`日志内容`则是每次调用log输出函数时传入的。

这就非常的妙。正好可以取log输出里的`函数名`作为重写的C程序中的函数的`附加名字`。

对于那些没有log输出的函数，就只能自己命名了。

### assert断言确保结构体大小、成员偏移量符合要求

利用assert函数，我们在使用逆向出来的结构体时，遇到sizeof求结构体大小时，可以将该大小与汇编中的常量进行断言比较，确保没有差错。

同样的，对于windows api中的结构体成员的偏移量，也可以跟汇编中的常量进行断言比较，确保没有差错。

其原型如下：
```c
#include <assert.h>

void assert(scalar expression);
```

### offsetof求取结构体成员的偏移量

当windows api已经帮我们定义好了一个结构体，我们可以使用 offsetof 宏来求取某个成员的偏移量，结合assert断言，确保跟汇编中的偏移量一致。

其原型如下：

```c
#include <stddef.h>

size_t offsetof(type, member);
```

## 参考文档

- ida sp-analysis failed: Igor’s tip of the week #27: Fixing the stack pointer, https://hex-rays.com/blog/igors-tip-of-the-week-27-fixing-the-stack-pointer/
- msdn win32 api: 你可以选择在搜索引擎上直接搜索。还有一个更方便的方法是在C程序里引用该api函数，鼠标点击该函数，按下F1，Visual Studio会自动打开该api函数的网页。
