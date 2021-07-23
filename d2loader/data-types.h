#pragma once

#include "pch.h"
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
#pragma pack(push, 1) // 逐字节对齐，方便定位。
// #pragma pack 指令用法详情见：https://docs.microsoft.com/en-us/cpp/preprocessor/pack?view=msvc-160
union program_setting_store
{
    struct
    {
        // 不允许使用不完整的类型
        // BYTE offset[0];
        BYTE value;
    } db_0000_expansion;

    struct
    {
        BYTE offset[4];
        BYTE value;
    } db_0004_window;

    struct
    {
        BYTE offset[5];
        BYTE value;
    } db_0005;

    struct
    {
        BYTE offset[6];
        BYTE value;
    } db_0006_glide;

    struct
    {
        BYTE offset[7];
        BYTE value;
    } db_0007_openGl;

    struct
    {
        BYTE offset[8];
        BYTE value;
    } db_0008_rave;

    struct
    {
        BYTE offset[9];
        BYTE value;
    } db_0009_d3d;

    struct
    {
        BYTE offset[9];
        BYTE value;
    } db_0009_perspective;

    struct
    {
        BYTE offset[0xa];
        BYTE value;
    } db_000a_lowQuality;

    struct
    {
        BYTE offset[0xb];
        DWORD value;
    } dd_000b_gamma;

    struct
    {
        // 根据 global_dd_402ea8 命令行参数表，gamma参数的偏移量为 0xc
        // 而在汇编代码中，gamma参数的偏移量为 0xb，
        // 两者冲突！
        // 从对齐的角度来看，DWORD 数据的偏移量为 0xc 更好。
        BYTE offset[0xc];
        DWORD value;
    } dd_000c_gamma;

    struct
    {
        BYTE offset[0xf];
        BYTE value;
    } db_000f_vSync;


    struct
    {
        BYTE offset[0x14];
        DWORD value;
    } dd_0014_gameType;

    struct
    {
        BYTE offset[0x1ff];
        BYTE value;
    } db_01ff_direct;

    struct
    {
        BYTE offset[0x20d];
        BOOL(*value)();
    } dd_020d_IsExpansion;

    struct
    {
        BYTE offset[0x21b];
        BYTE value;
    } db_021b_noSound;

    struct
    {
        BYTE offset[0x221];
        void* value;
    } dd_0221_comInt;

    struct
    {
        BYTE offset[0x79b];
        BYTE value;
    } db_079b_client;

    struct
    {
        BYTE offset[0x79c];
        BYTE value;
    } db_079c_server;

    struct
    {
        BYTE offset[0x79d];
        BYTE value;
    } db_079d_launch;

    struct
    {
        BYTE offset[0x79e];
        BYTE value;
    } db_079e_multi;

    struct
    {
        BYTE offset[0x79f];
        BYTE value;
    } db_079f_noTitle;

    struct
    {
        BYTE offset[0x7a0];
        // 是否启用 800x600 分辨率
        BYTE value;
    } db_07a0_res800;

    struct
    {
        BYTE offset[0x7a1];
        // 是否启用 640x480 分辨率
        BYTE value;
    } db_07a1_res640;

    struct
    {
        BYTE offset[0x7a6];
        // 是否禁用 d2char.mpq 相关数据
        BYTE value;
    } db_07a6_noChar;

    struct
    {
        BYTE offset[0x7a7];
        BYTE value;
    } db_07a7_clientExit;

    struct
    {
        BYTE offset[0x7a8];
        BYTE value;
    } db_07a8_multiClient;

    struct
    {
        BYTE offset[0x7a9];
        BYTE value;
    } db_07a9_printArg;

    struct
    {
        BYTE offset[0x7aa];
        BYTE value;
    } db_07aa_noScript;

    struct
    {
        BYTE offset[0x7ab];
        BYTE value;
    } db_07ab_noPlugin;

    struct
    {
        BYTE offset[0x7ac];
        BYTE value;
    } db_07ac_enableLogFile;

    struct
    {
        BYTE offset[0x7ae];
        BYTE value;
    } db_07ae_removeConfFile;

    struct
    {
        BYTE offset[0x7af];
        BYTE value;
    } db_07af_noCleanup;

    struct
    {
        BYTE offset[0x7b0];
        DWORD value;
    } dd_07b0_gameState;

    struct
    {
        BYTE offset[0x7b4];
        DWORD value;
    } dd_07b4_gameProductVersionFlag;

    // 进程的优先级。默认为 bnormal，即 低于正常。
    struct
    {
        BYTE offset[0x7bc];
        char value[7 + 1];
    } db_07bc_processPriority;

    struct
    {
        BYTE offset[0x7d4];
        char value[18];
    } db_07d4_locale;

    struct
    {
        BYTE offset[0x7ec];
        char value[9 + 1];
    } db_07ec_gameName;

    struct
    {
        BYTE offset[0x804];
        char value[0x80];
    } db_0804_title;

    struct
    {
        BYTE offset[0x884];
        char value[0x104];
    } db_0884_confFile;

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

    struct
    {
        BYTE offset[0xa8c];
        char value[0x104];
    } db_0a8c_pluginDir;

    struct
    {
        BYTE offset[0xb90];
        char value[0x104];
    } db_0b90_scriptFile;

    BYTE padding[0xc94];
};
#pragma pack(pop)

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

struct patch_search_item
{
    // 00 = 20 - 00
    const char* moduleFileName;
    // 04 = 20 - 1c
    DWORD v1;
    // 08 = 20 - 18
    DWORD v2;
    // 0c = 20 - 14
    void* v3;
    // 10 = 20 - 10
    DWORD v4;
    // 14 = 20 - 0c
    void* v5;
    // 18 = 20 - 08
    DWORD v6;
    // 1c = 20 - 04
    DWORD v7;
    // 20 = 20 - 00
    void* v8;
    // 24 = 20 + 04
    const char* pattern;
    // 28 = 20 + 08
    // 如果是搜索模式，那么记录找到的模式的地址
    void* patternLocation;
};
