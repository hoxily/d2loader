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
    // �����Ķ���
    const char* shortName;
    // ��������
    const char* category;
    // �����ĳ���
    const char* longName;
    // �ò����� program_setting_store �ṹ�е�ƫ����
    DWORD offset;
    // ����������
    DWORD type;
};

// ������ʹ��union��ֱ������÷Ѿ���˼���ֶε�ƫ�����ˡ�
#pragma pack(1) // ���ֽڶ��룬���㶨λ
union program_setting_store
{
    #pragma pack(1)
    struct
    {
        // ������ʹ�ò�����������
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
        // ���� global_dd_402ea8 �����в�����gamma������ƫ����Ϊ 0xc
        // ���ڻ������У�gamma������ƫ����Ϊ 0xb��
        // ���߳�ͻ��
        // �Ӷ���ĽǶ�������DWORD ���ݵ�ƫ����Ϊ 0xc ���á�
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
        // �Ƿ����� 800x600 �ֱ���
        BYTE value;
    } db_07a0_res800;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x7a1];
        // �Ƿ����� 640x480 �ֱ���
        BYTE value;
    } db_07a1_res640;

    #pragma pack(1)
    struct
    {
        BYTE offset[0x7a6];
        // �Ƿ���� d2char.mpq �������
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

    // ���̵����ȼ���Ĭ��Ϊ bnormal���� ����������
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
        * ��ض���һ��char���顣
        * ԭ���� 0x00406d4f ���Ļ�����ֱ�ӷ����˴˴��ĵ�һ���ַ���
        * �����charָ�룬����Ҫ��ȡ����ָ��Ȼ�����ָ��ָ����ڴ档�μ��·������룺
        * 
        * mov eax, global_dd_408620_settings
        * xor ebx, ebx
        * cmp [eax+988h], bl
        * 
        * ���� global_dd_402ea8_CommandLineArgumentTable �����˴������в����ĳ��ȡ�
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
