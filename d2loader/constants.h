#pragma once

// �˴��벻���ܱ�ִ��
#define ASSERT_UNREACHABLE_CODE (0)

// �������в����޾������ֵ
#define ARG_TYPE_NO_PARAM 1
// �������в���Ϊ16λ�޷���������֧��ʮ���ơ��˽��ơ�ʮ�����Ƶȶ��ֱ�﷽ʽ
#define ARG_TYPE_UNSIGNED_SHORT_INT 2
// �������в���Ϊ32λ�޷���������֧��ʮ���ơ��˽��ơ�ʮ�����Ƶȶ��ֱ�﷽ʽ
#define ARG_TYPE_UNSIGNED_INT 4
// ������ֵ��ʾ�ò���ֵ��Ϊ�ַ����Ĵ洢�������Ĵ�С

#define RUN_PLUGIN_REASON_INIT 0x1ul
#define RUN_PLUGIN_REASON_CLEANUP 0x2ul
#define RUN_PLUGIN_REASON_ENTER_GAME 0x4ul
#define RUN_PLUGIN_REASON_LEAVE_GAME 0x8ul
#define RUN_PLUGIN_REASON_ENTER_CHANNEL 0x10ul
#define RUN_PLUGIN_REASON_LEAVE_CHANNEL 0x20ul
#define RUN_PLUGIN_REASON_ENTER_MAIN_MENU 0x40ul
#define RUN_PLUGIN_REASON_LEAVE_MAIN_MENU 0x80ul
#define RUN_PLUGIN_REASON_ERROR_CLEANUP 0x100ul

#define LOG_TYPE_FILE 0x20
#define LOG_TYPE_CONSOLE 0x10

#define GAME_STATE_NONE 0
#define GAME_STATE_CLIENT 1
#define GAME_STATE_SERVER 2
#define GAME_STATE_MULTI 3
#define GAME_STATE_LAUNCH 4
#define GAME_STATE_INVALID 5