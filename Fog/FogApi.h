#pragma once

// 使用 Fog 的地方需要先定义 USE_FOG_API 然后再包含此头文件。
#ifdef USE_FOG_API
    #define FOG_API __declspec(dllimport)
#else
    #define FOG_API __declspec(dllexport)
#endif