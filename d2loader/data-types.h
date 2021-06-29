#pragma once

#include <Windows.h>

// fn_OnInitPlugin ������ջƽ���Ǻ����ڲ����ġ�
typedef void(__stdcall* fn_OnInitPlugin)(DWORD gameProductVersionFlag);

// fn_RunPlugin ������ջƽ���Ǻ����ڲ����ġ�
typedef BOOL(__stdcall* fn_RunPlugin)(DWORD reasonFlag, DWORD* gameProductVersionFlag);

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
        BYTE offset[0xa8c];
        char value[0x104];
    } db_0a8c_pluginDir;

    BYTE padding[0xc94];
};

// ���ڲ������������� __stdcall �� __cdecl һ�¡�
typedef struct query_interface_result* (__stdcall* fn_QueryInterface)();

typedef HMODULE(WINAPI* fn_LoadLibraryA)(_In_ LPCSTR lpLibFileName);

typedef DWORD(WINAPI* fn_GetModuleFileNameA)(
    _In_opt_ HMODULE hModule,
    _Out_ LPSTR lpFilename,
    _In_ DWORD nSize);

typedef HWND(WINAPI* fn_FindWindowA)(
    _In_opt_ LPCSTR lpClassName,
    _In_opt_ LPCSTR lpWindowName
    );

struct hook_search_item
{
    const char* functionName;
    void* functionProcAddress;
};