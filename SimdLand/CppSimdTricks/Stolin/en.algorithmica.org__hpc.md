## [The Art of SIMD Programming by Sergey Slotin](https://www.youtube.com/watch?v=vIRjSdTCIEU)


- 2x faster std::accumulate aka sum
- 10x faster std::find
- 1.5 faster std::count
- 10x faster std::min_element aka argmin
- 4x faster std::reverse
- 6-7x faster std::copy_if aka filter
- 2x faster std::bitset::count aka popcount
- 2.5x faster std::partial_sum aka prefix sum
- 50x faster scanf("%d")

(Benchmarked on Zen 2 and GCC 9.3)
---

_mm_add_epi16: add two 128-bit vectors of 8 16-bit integers
_mm256_acos_pd: calculate elementwise arccos for 4 packed doubles
_mm256_ceil_ps: round each of 8 floats to the nearest integer
_mm256_cmpeq_epi32: compare two 256-bit vectors of 8 32-bit integers for equality
_mm256_blendv_ps: blend two 256-bit vectors of 8 floats using a mask
_mm256_permutevar8x32_epi32: permute 8 32-bit integers in a 256-bit vector using a control mask

---
