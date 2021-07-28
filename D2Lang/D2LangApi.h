#pragma once

// 使用 D2Lang 的地方需要先定义 USE_D2_LANG_API 然后再包含此头文件。
#ifdef USE_D2_LANG_API
#define D2_LANG_API __declspec(dllimport)
#else
#define D2_LANG_API __declspec(dllexport)
#endif
