## 反编译目标文件

| 属性名 | 属性值 |
| - | - |
| 文件名 | D2Loader.exe |
| MD5 | 270B200034F60488E7B1E2A5EAC2085D |
| SHA1 | 797AF89EB546656FE875794D4F703D6E03374813 |
| SHA256 | 69BD9820AF5E494E7C4C0FA8B10FE0FF2784E0EA2BA9358499EB3CB1678F8F07 |
| 大小 | 38,912 字节 |

## 反编译工具介绍
IDA 

## 标识符命名规范
全局变量

函数名称

局部变量

## 遇到的困难

## 编码技巧

巧用 union 联合体做结构体偏移。

利用log中的tag标签为函数取名。

assert断言确保结构体大小符合要求。

offsetof求取结构体成员的偏移量。

## 参考文档

ida sp-analysis failed:

msdn win32 api:
