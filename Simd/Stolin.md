https://en.algorithmica.org/hpc
https://github.com/sstolin/amh-code

## [The Art of SIMD Programming by Sergey Slotin](https://www.youtube.com/watch?v=vIRjSdTCIEU)

Most SIMD instrinsics follow the _mm<size>_<action>_<type> naming convention:

* _mm_add_epi16: add two 128-bit vectors of 16-bit extended packed integers
* _mm256_acos_pd: calculate elementwise across the 4 packed doubles
* _mm256_ceil_ps: round up each of 8 floats to the nearest integer
* _mm256_broadcast_sd: copy a double from memory to 4 elements of a vector
* _mm256_cmpeq_epi32: compare 8+8 packed ints and return  a  256-bit mask
* _mm256_blendv_ps: pick elements from one of two vectors according to a mask
* _mm256_permutevar8x32_epi32: pick elements by indices from a vector


---
