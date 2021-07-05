#pragma once

#include <Windows.h>
#include "data-types-function-ptr.h"

struct query_interface_result
{
    // 0x44320000
    DWORD magic;
    // 0x01000912
    DWORD version;
    const char* pluginName;
    fn_RunPlugin runPlugin;
};

struct query_interface_result_old
{
    // 0x44320000
    DWORD magic;
    const char* pluginName;
    fn_OnInitPlugin init;
};

struct loaded_plugin_item
{
    DWORD dd_0000_activeFlag;
    HMODULE dd_0004_dllModule;
    HANDLE dd_0008_autoResetEventHandle;
    char* dd_000c_dllFileName;
    struct query_interface_result* dd_0010_queryInterfaceResult;
};

#pragma pack(1)
struct string_index_item
{
    // 参数的短名
    const char* shortName;
    // 参数分类
    const char* category;
    // 参数的长名
    const char* longName;
    // 该参数在 program_setting_store 结构中的偏移量
    DWORD offset;
    // 参数的类型
    DWORD type;
};

// 在这里使用union简直绝妙！不用费劲心思凑字段的偏移量了。
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

    #pragma pack(1)
    struct
    {
        BYTE offset[0x20d];
        BOOL(*value)();
    } dd_020d_IsExpansion;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x79d];
        BYTE value;
    } db_079d;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x7a9];
        BYTE value;
    } db_07a9_printArg;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x7aa];
        BYTE value;
    } db_07aa_noScript;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x7ab];
        BYTE value;
    } db_07ab_noPlugin;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x7ac];
        BYTE value;
    } db_07ac_enableLogFile;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x7ae];
        BYTE value;
    } db_07ae_removeConfFile;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x7af];
        BYTE value;
    } db_07af_noCleanup;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x7b4];
        DWORD value;
    } dd_07b4_gameProductVersionFlag;

    // 进程的优先级。默认为 bnormal，即 低于正常。
    #pragma pack(1)
    struct
    {
        BYTE offset[0x7bc];
        char value[7 + 1];
    } db_07bc_processPriority;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x7ec];
        char value[9 + 1];
    } db_07ec_gameName;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x804];
        char value[38 + 1];
    } db_0804_title;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x884];
        char value[0x104];
    } db_0884_confFile;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x988];
        /*
        * 这必定是一个char数组。
        * 原因是 0x00406d4f 处的汇编代码直接访问了此处的第一个字符。
        * 如果是char指针，则需要先取得针指，然后访问指针指向的内存。参见下方汇编代码：
        * 
        * mov eax, global_dd_408620_settings
        * xor ebx, ebx
        * cmp [eax+988h], bl
        * 
        * 由于长度未知，暂时定为1.
        */
        char value[1];
    } db_0988_extraMpq;

    #pragma pack(1)
    struct
    {
        BYTE offset[0xa8c];
        char value[0x104];
    } db_0a8c_pluginDir;

    BYTE padding[0xc94];
};

struct hook_search_item
{
    const char* functionName;
    void* functionProcAddress;
};