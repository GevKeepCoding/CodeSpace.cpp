#pragma once

namespace SimdSupportSettings
{
inline static bool IsSSE_supported = __builtin_cpu_supports("sse");
inline static bool IsSSE2_supported = __builtin_cpu_supports("sse2");
inline static bool IsAVX_supported = __builtin_cpu_supports("avx");
inline static bool IsAVX2_supported = __builtin_cpu_supports("avx2");
inline static bool IsAVX512_supported = __builtin_cpu_supports("avx512f");
// cat /proc/cpuinfo
}

