#pragma once

// ʹ�� D2Lang �ĵط���Ҫ�ȶ��� USE_D2_LANG_API Ȼ���ٰ�����ͷ�ļ���
#ifdef USE_D2_LANG_API
#define D2_LANG_API __declspec(dllimport)
#else
#define D2_LANG_API __declspec(dllexport)
#endif
