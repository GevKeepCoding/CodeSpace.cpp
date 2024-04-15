
## [SIMD Libraries in C++ - Jeff Garland - CppNow 2023](https://www.youtube.com/watch?v=hlgCeWC9jxI)

* https://simdjson.org
* simd src https://github.com/neurolabusc/simd_crc
* simd sort https://github.com/intel/x86-simd-sort
* math https://bitbucket.org/blaze-lib/blaze/src/master

### Developer libraries
xsimd c++ 11: https://readthedocs.io/en/latest
eve c++ 20: https://jfalcou/github.io/eve/index.html
Agner Fog VCL https://www.agner.org/optimize/vectorclass.pdf



----

TODO: practice simd functions:
[SIMD and vectorization using AVX intrinsic functions (Tutorial)
](https://www.youtube.com/watch?v=AT5nuQQO96o)


```C++
__m256 _mm256_i32gather_ps(float* const source, __m256i indices,
						   const int scale)
Gather memory address = source + indices * scale

```

---