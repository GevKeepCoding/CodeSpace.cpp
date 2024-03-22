#pragma once


#include <benchmark/benchmark.h>
#include <bit>
#include <immintrin.h>
#include <xutility>
#include <stdio.h>
#include <print>
#include <print>
#include <iostream>
//The Art of SIMD Programming by Sergey Slotin https://www.youtube.com/watch?v=vIRjSdTCIEU


#pragma once

//import std;
#include <immintrin.h>
#include <print>
#include <iostream>
#include <bit>
#include <zmmintrin.h>
#include <intrin0.inl.h>
/*

The Art of SIMD Programming by Sergey Slotin : https://www.youtube.com/watch?v=vIRjSdTCIEU
The Book: https://en.algorithmica.org

#pragma GCC target("avx2")
#pragma GCC optimize("O3")

#include <x86intrin.h>
#include <bits/stdc++.h>

or in cmd: -O3 -march=native or -mavx2


_mm_add_epi16: add two 128-bit vectors of 8 16-bit integers
_mm256_acos_pd: calculate elementwise arccos for 4 packed doubles
_mm256_ceil_ps: round each of 8 floats to the nearest integer
_mm256_cmpeq_epi32: compare two 256-bit vectors of 8 32-bit integers for equality
_mm256_blendv_ps: blend two 256-bit vectors of 8 floats using a mask
_mm256_permutevar8x32_epi32: permute 8 32-bit integers in a 256-bit vector using a control mask
*/

namespace Stolin1
{
	using reg = __m256i;
	void sum1(int* a, int* b, int* c, int n) {
		// vectorized part
		for (size_t i = 0; i + 7 < n; i += 8)
		{
			reg x = _mm256_loadu_si256((__m256i*) & a[i]);
			reg y = _mm256_loadu_si256((__m256i*) & b[i]);
			reg z = _mm256_add_epi32(x, y);
			_mm256_storeu_si256((__m256i*) & c[i], z);
		}

		// scalar part
		for (size_t i = n / 8 * 8; i < n; ++i)
		{
			c[i] = a[i] + b[i];
		}

	}
	// We will assume that everything is properly aligned.
	// Reduction
	int sum2(int* a, int n) {
		int s = 0;
		for (size_t i = 0; i < n; i++)
		{
			s += a[i];
		}
		return s;
	}

	// Let's make above code faster
	int hsum(reg x)//horizontal sam
	{
		int t[8], s = 0;
		_mm256_storeu_si256((reg*)t, x);
		for (int i = 0; i < 8; ++i) {
			s += t[i];
		}
		return s;
	}

	int sum3(int* a, int n) {
		reg s = _mm256_setzero_si256();

		// "vertical summation"
		for (size_t i = 0; i < n; i += 8)
		{
			s = _mm256_add_epi32(s, _mm256_loadu_si256((__m256i*) & a[i]));
		}
		return hsum(s);
	}

	int sum4(int* a, int n) {
		reg s0 = _mm256_setzero_si256();
		reg s1 = _mm256_setzero_si256();

		for (int i = 0; i < n; i += 16) {
			s0 = _mm256_add_epi32(s0, _mm256_loadu_si256((__m256i*) & a[i]));
			s1 = _mm256_add_epi32(s1, _mm256_loadu_si256((__m256i*) & a[i + 8]));
		}
		return hsum(_mm256_add_epi32(s0, s1));
	}

	// Let's make above hsum code faster
	int hsum_fastest(reg x)//horizontal sam
	{
		reg t = _mm256_hadd_epi32(x, x);
		t = _mm256_hadd_epi32(t, t);
		return _mm256_extract_epi32(t, 0) + _mm256_extract_epi32(t, 4);
	}

	// ----------------------------------
	// for (int i =0; i < N;++i) 
	// s += (a[i] < 50 ? a[i] : 0);

	void Sum_of_less_Than_50(int* a, int n) {
		const reg c = _mm256_set1_epi32(50);
		const reg z = _mm256_setzero_si256();

		reg s = _mm256_setzero_si256();

		for (int i = 0; i < n; i += 8) {
			reg x = _mm256_loadu_si256((__m256i*) & a[i]); // [23, 71, 90,40]
			reg mask = _mm256_cmpgt_epi32(c, x); // [-1, 0, 0, -1]
			x = _mm256_blendv_epi8(z, x, mask); // [23, 0, 0, 40]
			s = _mm256_add_epi32(s, x);
		}
	}

	// ----------------------------------
	// for (int i =0; i < N;++i)
	//		if (a[i] == x) return i;
	// return -1;

	int find(int* a, int n, int needle) {
		reg x = _mm256_set1_epi32(needle);

		for (int i = 0; i < n; i += 8) {
			reg y = _mm256_loadu_si256((__m256i*) & a[i]);
			reg m = _mm256_cmpeq_epi32(x, y);
			std::uint32_t mask = _mm256_movemask_ps(_mm256_castsi256_ps(m));
			if (mask) {
				return i + std::countr_zero(mask);// __builtin_ctz(mask);
			}
		}
		return -1;
	}

	// let's make it faster by using _mm256_testz_si256(checks if a vector is zero)
	int find2(int* a, int n, int needle) {
		reg x = _mm256_set1_epi32(needle);

		for (int i = 0; i < n; i += 8) {
			reg y = _mm256_loadu_si256((__m256i*) & a[i]);
			reg m = _mm256_cmpeq_epi32(x, y);
			if (!_mm256_testz_si256(m, m)) {
				std::uint32_t mask = _mm256_movemask_ps(_mm256_castsi256_ps(m));
				return i + std::countr_zero(mask);
			}
		}
		return -1;
	}// this is not faster because of _mm256_castsi256_ps and movemask : they both have low throughput for now
	// ----------------------------------
	// But we can do multiple of them in parallel:
	int find3(int* a, int n, int needle) {
		reg x = _mm256_set1_epi32(needle);

		for (int i = 0; i < n; i += 16) {
			reg y1 = _mm256_loadu_si256((__m256i*) & a[i]);
			reg y2 = _mm256_loadu_si256((__m256i*) & a[i + 8]);
			reg m1 = _mm256_cmpeq_epi32(x, y1);
			reg m2 = _mm256_cmpeq_epi32(x, y2);
			reg m = _mm256_or_si256(m1, m2);
			if (!_mm256_testz_si256(m, m)) {
				std::uint32_t mask;
				if (!_mm256_testz_si256(m1, m1))
				{
					mask = _mm256_movemask_ps(_mm256_castsi256_ps(m1));
					return i + std::countr_zero(mask);
				}
				else {
					mask = _mm256_movemask_ps(_mm256_castsi256_ps(m2));
					return i + 8 + std::countr_zero(mask);
				}
			}
		}
		return -1;
	} // perf changed from: 20 => 34 GFLOPS

	// ----------------------------------
	reg cmp(reg x, int* p) {
		reg y = _mm256_load_si256((__m256i*)p);
		return _mm256_cmpeq_epi32(x, y);
	}

	int find4(int* a, int n, int needle) {
		reg x = _mm256_set1_epi32(needle);

		for (int i = 0; i < n; i += 32) {
			reg m1 = cmp(x, &a[i]);
			reg m2 = cmp(x, &a[i + 8]);
			reg m3 = cmp(x, &a[i + 16]);
			reg m4 = cmp(x, &a[i + 24]);
			reg m12 = _mm256_or_si256(m1, m2);
			reg m34 = _mm256_or_si256(m3, m4);
			reg m = _mm256_or_si256(m12, m34);
			if (!_mm256_testz_si256(m, m)) {

			}
		}
		return -1;
	} // perf changed from: 34 => 43 GFLOPS (10x faster than scalar implementation)

	auto example()
	{
		using reg = __m256i;

		// set a to  [23, 71, 90,40]
		reg c = _mm256_set1_epi32(50); // [50, 50, 50, 50, 50, 50, 50, 50]
		reg a = _mm256_setr_epi32(23, 71, 90, 40, 0, 0, 0, 0);
		std::cout << a.m256i_i32[0];
		reg mask = _mm256_cmpgt_epi32(c, a);
		reg z = _mm256_setzero_si256();
		a = _mm256_blendv_epi8(z, a, mask); // [23, 0, 0, 40]
		reg b = _mm256_hadd_epi32(a, a);

		return hsum_fastest(b);
	}
	//----------------------------------
	//------		ARGMIN
	int argmin(int* a, int n) {
		int k = 0;

		for (int i = 0; i < n; ++i) {
			if (a[i] < a[k])
				k = i;
		}
		return k;
	} // ~ 1.5 GFLOPS

	// Let's make it faster
	// implement: min and find 
	int min(int* a, int n) {
		return *std::min_element(a, a + n);
	}

	const int B = 256;
	std::pair<int, int> approx_argmin(int* a, int n) {
		int res = std::numeric_limits<int>::max(), idx = 0;

		for (int i = 0; i < n; i += B) {
			int val = min(a + i, B);//todo: implement my own effective min
			if (val < res) {
				res = val;
				idx = i;
			}
		}
		return {res, idx};// the min and it's first block.
	}

	int argminv2(int* a, int n) {
		auto [needle, base] = approx_argmin(a, n);
		int idx = find(a + base, B, needle);
		return base + idx;
	}// ~ 22 /~19 GFLOPS


	//------		ARGMIN : NOT TESTED
	int argmin3(int* a, int n) {
		reg min = _mm256_set1_epi32(a[0]);
		reg idx = _mm256_setzero_si256();

		for (int i = 0; i < n; i += 8) {
			reg x = _mm256_loadu_si256((__m256i*) & a[i]);
			reg mask = _mm256_cmpgt_epi32(min, x);
			min = _mm256_blendv_epi8(min, x, mask);
			reg m = _mm256_cmpeq_epi32(min, x);
			idx = _mm256_blendv_epi8(idx, _mm256_set1_epi32(i), m);
		}
		reg m = _mm256_permutevar8x32_epi32(idx, _mm256_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7));
		reg m1 = _mm256_min_epi32(m, _mm256_permutevar8x32_epi32(m, _mm256_setr_epi32(1, 0, 3, 2, 5, 4, 7, 6)));
		reg m2 = _mm256_min_epi32(m1, _mm256_permutevar8x32_epi32(m1, _mm256_setr_epi32(2, 3, 0, 1, 6, 7, 4, 5)));
		reg m3 = _mm256_min_epi32(m2, _mm256_permutevar8x32_epi32(m2, _mm256_setr_epi32(4, 5, 6, 7, 0, 1, 2, 3)));
		return _mm256_extract_epi32(m3, 0);
	}


	//----------------------------------
	// ------		reverse
	void reverse_scalar(int* a, int n) {
		for (size_t i = 0; i < n / 2; i++)
		{
			std::swap(a[i], a[n - i - 1]);
		}
	}// 1.8 GFLOPS



	// to make faster we will use _mm256_permutevar8x32_epi32
	const reg reversePermutationMask = _mm256_setr_epi32(7, 6, 5, 4, 3, 2, 1, 0);

	void reverse_vector(int* a, int n) {
		for (size_t i = 0; i < n / 2; i += 8)
		{
			reg x = _mm256_loadu_si256((__m256i*) & a[i]);
			reg y = _mm256_loadu_si256((__m256i*) & a[n - i - 8]);

			_mm256_permutevar8x32_epi32(x, reversePermutationMask);
			_mm256_permutevar8x32_epi32(y, reversePermutationMask);

			_mm256_storeu_si256((__m256i*) & a[i], y);
			_mm256_storeu_si256((__m256i*) & a[n - i - 8], x);
		}
	}// 7 GFLOPS (3.75x faster)


	//				FILTERING
	int filter1(int* a, int n,
				int* b, int p)
	{
		int k = 0;
		for (size_t i = 0; i < n; i++)
		{
			if (a[i] < p) {
				b[k++] = a[i];
			}
		}
		return k;
	} // 0.7 - 1.5 GFLOPS depending on P

	struct Precalc {

		alignas(64) int permutation[256][8];

		constexpr Precalc() : permutation{} {

			for (size_t m = 0; m < 256; m++)
			{
				int k = 0;
				for (int i = 0; i < 8; i++)
					if ((m >> i) & 1) // or m & (1 << i)
						permutation[m][k++] = i;
			}
		}
	};


	constexpr Precalc T;

	int filter_vector(int* a, int n, int* b, int P)
	{
		int k = 0;
		const reg p = _mm256_set1_epi32(P);

		for (size_t i = 0; i < n; i += 8)
		{
			reg x = _mm256_loadu_si256((__m256i*) & a[i]);
			reg m = _mm256_cmpgt_epi32(p, x);
			int mask = _mm256_movemask_ps(_mm256_castsi256_ps(m));

			reg permutation = _mm256_load_si256((__m256i*) & T.permutation[mask]);
			x = _mm256_permutevar8x32_epi32(x, permutation);

			_mm256_storeu_si256((__m256i*) & b[k], x);
			k += _mm_popcnt_u32(mask);
			// or std::popcount(mask);
		}
		return k;
	}// 3 - 7x faster than scalar implementation

	 // filter with avx512: it has dedicated compress instruction=> much faster on AVX-512
	int filter_avx512_using_compress_instruction(int* a, int n, int* b, int P) {

		int k = 0;
		const __m512i p = _mm512_set1_epi32(P);
		const __m512i zero = _mm512_setzero_si512();
		for (size_t i = 0; i < n; i += 16) {
			__m512i x = _mm512_loadu_si512((__m512i*)&a[i]);
			__mmask16 m = _mm512_cmplt_epi32_mask(x, p);
			__m512i y = _mm512_mask_compress_epi32(zero, m, x);
			_mm512_storeu_si512((__m512i*)&b[k], y);
			k += std::popcount(m);
		}
		return k;
	}
	// More info: Wojciech Mula: https://www.youtube.com/watch?v=8g8Zg6G8f9A
	// Daniel Lemire: https://www.youtube.com/watch?v=8g8Zg6G8f9A
	//----------------------------------
	//32 * 16 = 512 bits
	// 128 / 16 = 8
}





namespace Optimizing_Binary_Tree_Stolin
{
	const int n = 100;
	int a[n];
	int lower_bound(int x) {

		int l = 0, r = n - 1;
		while (l < r) {
			int m = (l + r) / 2;
			if (a[m] >= x)
				r = m;
			else
				l = m + 1;
		}
		return a[l];
	}

	//


	// B = 16 compute the lower bound
	int rank1(int x, int* node, int B = 16) {
		for (int i = 0; i < B; i++) {
			if (node[i] >= x)
				return i;
		}
		return B;
	}
	int rank2(int x, int* node, int B = 16) {

		uint32_t mask = (1 << B);
		for (int i = 0; i < B; i++)
			mask |= (node[i] >= x) << i;
		return std::countr_zero(mask) - 1;
		//return __builtin_ffs(mask) - 1;//ffs stands for: "find first set" bit.
	}// not better than above one but using SIMD it will get better

	using reg = __m256i;
	int cmp(reg x_vec, int* y_ptr)//avx2
	{
		reg y_vec = _mm256_load_si256((reg*)y_ptr); // load 8 sorted elements
		reg mask = _mm256_cmpgt_epi32(x_vec, y_vec);// compare against the key
		return _mm256_movemask_ps(_mm256_castsi256_ps(mask));// extract the 8-bit mask
	}

	int rank3Simd(reg x, int* node, int B = 16) {
		uint32_t mask = ~(
			cmp(x, node) + (cmp(x, node + 8) << 8)
			);

		//return __builtin_ffs(mask) - 1;
		return 0;
	}






}



bool test = [] {

	volatile int a = INT_MAX;
	volatile unsigned int b = INT_MAX;

	print(std::cout,"fun {}\n", a * 7 / 7);
	print(std::cout,"fun {}\n", b * 7 / 7);

	return true;
};