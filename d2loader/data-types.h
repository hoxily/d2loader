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
        BYTE value;
    } db_0004_window;

    #pragma pack(1)
    struct
    {
        BYTE offset[5];
        BYTE value;
    } db_0005;

    #pragma pack(1)
    struct
    {
        BYTE offset[6];
        BYTE value;
    } db_0006_glide;

    #pragma pack(1)
    struct
    {
        BYTE offset[7];
        BYTE value;
    } db_0007_openGl;

    #pragma pack(1)
    struct
    {
        BYTE offset[8];
        BYTE value;
    } db_0008_rave;

    #pragma pack(1)
    struct
    {
        BYTE offset[9];
        BYTE value;
    } db_0009_d3d;

    #pragma pack(1)
    struct
    {
        BYTE offset[9];
        BYTE value;
    } db_0009_perspective;

    #pragma pack(1)
    struct
    {
        BYTE offset[0xa];
        BYTE value;
    } db_000a_lowQuality;

    #pragma pack(1)
    struct
    {
        BYTE offset[0xb];
        DWORD value;
    } dd_000b_gamma;

    #pragma pack(1)
    struct
    {
        // 根据 global_dd_402ea8 命令行参数表，gamma参数的偏移量为 0xc
        // 而在汇编代码中，gamma参数的偏移量为 0xb，
        // 两者冲突！
        // 从对齐的角度来看，DWORD 数据的偏移量为 0xc 更好。
        BYTE offset[0xc];
        DWORD value;
    } dd_000c_gamma;

    #pragma pack(1)
    struct
    {
        BYTE offset[0xf];
        BYTE value;
    } db_000f_vSync;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x1ff];
        BYTE value;
    } db_01ff_direct;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x20d];
        BOOL(*value)();
    } dd_020d_IsExpansion;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x21b];
        BYTE value;
    } db_021b_noSound;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x221];
        void* value;
    } dd_0221_comInt;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x79b];
        BYTE value;
    } db_079b_client;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x79c];
        BYTE value;
    } db_079c_server;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x79d];
        BYTE value;
    } db_079d_launch;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x79e];
        BYTE value;
    } db_079e_multi;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x79f];
        BYTE value;
    } db_079f_noTitle;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x7a0];
        // 是否启用 800x600 分辨率
        BYTE value;
    } db_07a0_res800;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x7a1];
        // 是否启用 640x480 分辨率
        BYTE value;
    } db_07a1_res640;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x7a6];
        // 是否禁用 d2char.mpq 相关数据
        BYTE value;
    } db_07a6_noChar;

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
        BYTE offset[0x7b0];
        DWORD value;
    } dd_07b0;

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
        char value[0x80];
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
        * 另外 global_dd_402ea8_CommandLineArgumentTable 定义了此命令行参数的长度。
        */
        char value[0x104];
    } db_0988_mpqFile;

    #pragma pack(1)
    struct
    {
        BYTE offset[0xa8c];
        char value[0x104];
    } db_0a8c_pluginDir;

    #pragma pack(1)
    struct
    {
        BYTE offset[0xb90];
        char value[0x104];
    } db_0b90_scriptFile;

    BYTE padding[0xc94];
};

struct hook_search_item
{
    const char* functionName;
    void* functionProcAddress;
};

struct d2_dll_function_item
{
    HMODULE* module;
    void **procedure;
    int ordinal;
};
