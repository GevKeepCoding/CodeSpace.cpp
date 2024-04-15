#pragma once
#include <immintrin.h>

// Function to perform (a + b) % c using AVX2
void mod_add_avx2(int* a, int* b, int* c, int* result, int length) {
    __m256i vec_c = _mm256_loadu_si256((__m256i*)c);
    for (int i = 0; i < length; i += 8) {
        __m256i vec_a = _mm256_loadu_si256((__m256i*) & a[i]);
        __m256i vec_b = _mm256_loadu_si256((__m256i*) & b[i]);

        __m256i vec_sum = _mm256_add_epi32(vec_a, vec_b);
        __m256i vec_mod = _mm256_rem_epi32(vec_sum, vec_c);

        _mm256_storeu_si256((__m256i*) & result[i], vec_mod);
    }
}

