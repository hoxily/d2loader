#pragma once

// ʹ�� Storm �ĵط���Ҫ�ȶ��� USE_STORM_API Ȼ���ٰ�����ͷ�ļ���
#ifdef USE_STORM_API
    #define STORM_API __declspec(dllimport)
#else
    #define STORM_API __declspec(dllexport)
#endif
