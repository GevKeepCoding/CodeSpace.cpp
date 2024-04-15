#pragma once
#include <string>
#include <zmmintrin.h>

namespace simd_hash
{
// google  highway hash algorithm: https://github.com/google/highwayhash
/*

HighwayHash is faster than SipHash for all input sizes,
with 5 times higher throughput at 1 KiB



SipHash is a fast but
'cryptographically strong' pseudo-random function
by Aumasson and Bernstein [https://www.131002.net/siphash/siphash.pdf].

SipHash will give you a 64-bit hash value for a given key and message.
And when called on same input, it will give you the same hash value.

HighwayHash is a fast, strong, and cross-platform hash function.
It is faster than SipHash and CityHash, and is as strong as SipHash.

1.	Speed: HighwayHash is faster than many other hash functions, including SipHash and CityHash. It provides high throughput, especially for larger input sizes.
2.	Cryptographic Strength: Despite its speed, HighwayHash maintains a high level of cryptographic strength. This makes it suitable for scenarios where both performance and security are important.
3.	Cross-Platform: HighwayHash is designed to work across different platforms and architectures. This makes it a versatile choice for a variety of applications.
4.	Consistency: Like other hash functions, HighwayHash will always produce the same hash value for the same input. This is a fundamental property of hash functions and is critical for their use in data structures like hash tables.
5.	AVX-512 Utilization: The HighwayHash algorithm can take advantage of the AVX-512 instruction set (where available) to perform efficient SIMD computations. This contributes to its high performance.


*/

// Interestingly, it is about twice as fast as a SIMD implementation using SSE4.1 (https://goo.gl/80GBSD).
// This is presumably due to the lack of SIMD bit rotate instructions prior to AVX-512.
void ExampleOfAVX512_bit_rotate()
{
    std::string input = "large input";
    __m512i input_vector = _mm512_loadu_si512(reinterpret_cast<const __m512i*>(input.data()));
    __m512i rotated = _mm512_rol_epi64(input_vector, 1);

}



}