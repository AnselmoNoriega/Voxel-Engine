#pragma once


#ifdef ENGINE_ENABLE_ASSERTS
    #define ASSERT(x, ...) { if(!(x)) { ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
    #define CORE_ASSERT(x, ...) { if(!(x)) { CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
    #define ASSERT(x, ...)
    #define CORE_ASSERT(x, ...)
#endif