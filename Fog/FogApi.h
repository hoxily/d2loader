#pragma once

// ʹ�� Fog �ĵط���Ҫ�ȶ��� USE_FOG_API Ȼ���ٰ�����ͷ�ļ���
#ifdef USE_FOG_API
    #define FOG_API __declspec(dllimport)
#else
    #define FOG_API __declspec(dllexport)
#endif