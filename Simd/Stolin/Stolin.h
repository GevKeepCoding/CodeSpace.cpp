#pragma once

#include <immintrin.h>
import std;
/*

The Art of SIMD Programming by Sergey Slotin

#pragma GCC target("avx2")
#pragma GCC optimize("O3")

#include <x86intrin.h>
#include <bits/stdc++.h>

or in cmd: -O3 -march=native or -mavx2
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


}