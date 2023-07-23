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


namespace Stolin {

	// if we write for gcc: we can just add #pragma GCC target("avx2")

	int s = 0;
	void avx2Perf(benchmark::State& state)
	{
		srand(1); auto N = state.range(0);
		std::vector<unsigned long> v1(N);
		for (int64_t i = 0; i < N; ++i) {
			v1[i] = rand();
		}

		for (auto _ : state) {
			for (int i = 0; i < N; ++i) {
				s += v1[i];
			}
		}
		state.SetItemsProcessed(N * state.iterations());
	}
	//BENCHMARK(avx2Perf)->Arg(1 << 22);

	void iterate_in_blocks_of_4() {
		double a[100], b[100], c[100];
		for (int i = 0; i < 100; i += 4) {
			__m256d x = _mm256_loadu_pd(&a[i]);
			__m256d y = _mm256_loadu_pd(&b[i]);

			__m256d z = _mm256_add_pd(x, y);
			_mm256_storeu_pd(&c[i], z);
		}
	}

	using reg = __m256i;

	void sum1(int* a, int* b, int* c, int n)
	{
		//vectorized part
		for (int i = 0; i + 7 < n; i += 8) {
			reg x = _mm256_loadu_epi32(&a[i]);
			reg y = _mm256_loadu_epi32(&b[i]);
			reg z = _mm256_add_epi32(x, y);
			_mm256_storeu_epi32(&c[i], z);
		}

		// scalar part
		for (int i = n / 8 * 8; i < n; ++i) {
			c[i] = a[i] + b[i];
		}
	}
	//--------------
	int hsum(reg x)
	{
		int t[8], s = 0;
		_mm256_storeu_si256((reg*)t, x);

		for (int i = 0; i < 8; ++i)
			s += t[i];

		return s;
	}

	int sum2(int* a, int n)
	{
		reg s = _mm256_setzero_si256();
		// vertical summation
		for (int i = 0; i < n; i += 8)
			s = _mm256_add_epi32(s, _mm256_load_si256((reg*)&a[i]));

		return hsum(s);// horizontal summation
	}

	// 2x faster accumulate:
	int sum3(int* a, int n) {

		reg s0 = _mm256_setzero_si256();
		reg s1 = _mm256_setzero_si256();

		for (int i = 0; i < n; i += 16) {
			s0 = _mm256_add_epi32(s0, _mm256_loadu_epi32((reg*)&a[i]));
			s1 = _mm256_add_epi32(s1, _mm256_loadu_epi32((reg*)&a[i + 8]));
		}
		return hsum(_mm256_add_epi32(s0, s1));
	}// some newer compiler may do above optimizations, but not all.

	void MaskingExample() {

		for (int i = 0; i < 100; i++)
			a[i] = rand() % 100;
		int s = 0;
		// branch:
		for (size_t i = 0; i < 100; i++)
			if (a[i] < 50) s += a[i];

		// no branch:
		for (size_t i = 0; i < 100; i++)
		{
			s += (a[i] < 50) * a[i];
		}

		// also no branch:
		for (int i = 0; i < 50; ++i) {
			s += (a[i] < 50 ? a[i] : 0);
		}
	}

	// let's write better version of filtering.
	void Better_filter50ThenSum(int* a, int n) {

		const reg c = _mm256_set1_epi32(50);
		const reg z = _mm256_setzero_si256();

		reg s = _mm256_setzero_si256();

		for (int i = 0; i < n; i += 8) {

			reg x = _mm256_loadu_epi32(&a[i]);
			reg mask = _mm256_cmpgt_epi32(c, x);
			x = _mm256_blendv_epi8(z, x, mask);
			s = _mm256_add_epi32(s, x);
		}

	}
	auto load_from(float const* psrc)
	{
		return _mm512_loadu_ps(psrc);
	}

	__m256 psfrom(__m256i x) {
		return _mm256_castsi256_ps(x);
	}

	// _mm256_movemask_ps: take first bits of each 32-bit element and return a 8-bit mask
	int find1(int* a, int n, int needle) {

		reg x = _mm256_set1_epi32(needle);
		for (size_t i = 0; i < n; i += 8)
		{
			reg y = _mm256_load_si256((reg*)&a[i]);
			reg m = _mm256_cmpeq_epi32(x, y);
			int mask = _mm256_movemask_ps(psfrom(m));

			if (mask) {
				return i + std::countr_zero(static_cast<unsigned>(mask));
			}// count trailing zeroes __builtin_ctz "find lowest set bit"
		}
		return -1;
	}// 5x faster than scalar one

	static inline int __builtin_ctz(unsigned x)
	{
		return (int)_tzcnt_u32(x);
	}

	// _mm256_testz_si256 can be used to check if a vector is zero (similar to the scalar test)

	int find2(int* a, int n, int needle) {

		reg x = _mm256_set1_epi32(needle);
		for (int i = 0; i < n; i += 8) {

			reg y = _mm256_load_si256((reg*)&a[i]);
			reg m = _mm256_cmpeq_epi32(x, y);

			if (!_mm256_testz_si256(m, m)) {
				unsigned mask = _mm256_movemask_ps(psfrom(m));
				return i + std::countr_zero(mask);
			}
		}

		return -1;
	}// no perf gain because both movemask and testz are low throughput operations

	int find3(int* a, int n, int needle)
	{
		reg x = _mm256_set1_epi32(needle);

		for (int i = 0; i < n; i += 16) {

			reg y1 = _mm256_load_si256((reg*)&a[i]);
			reg y2 = _mm256_load_si256((reg*)&a[i + 8]);
			reg m1 = _mm256_cmpeq_epi32(x, y1);
			reg m2 = _mm256_cmpeq_epi32(x, y2);
			reg m = _mm256_or_si256(m1, m2);
			if (!_mm256_testz_si256(m, m)) {

			}
		}
		return -1;
	}// 20 -> 34 GFLOPS

	reg cmpeq(reg x, int* p)
	{
		reg y = _mm256_load_si256((reg*)p);
		return _mm256_cmpeq_epi32(x, y);
	}

	int find4(int* a, int n, int needle)
	{
		reg x = _mm256_set1_epi32(needle);

		for (int i = 0; i < n; i += 32) {

			reg m1 = cmpeq(x, &a[i]);
			reg m2 = cmpeq(x, &a[i + 8]);
			reg m3 = cmpeq(x, &a[i + 16]);
			reg m4 = cmpeq(x, &a[i + 24]);

			reg m12 = _mm256_or_si256(m1, m2);
			reg m34 = _mm256_or_si256(m3, m4);
			reg m = _mm256_or_si256(m12, m34);
			if (!_mm256_testz_si256(m, m)) {
				//...
			}
		}
		return -1;
	}// 43 GFLOPS (10x faster than the original scalar implementation)


	//--------			ARGMIN

	int argmin(int* a, int n) {
		int k = 0;
		for (int i = 0; i < n; ++i) {
			if (a[i] < a[k])
				k = i;
		}
		return k;
	}// 1.5 GFLOPS
	/*
	const int B = 256;
	std::pair<int, int> approx_argmin(int* a, int n)
	{
		int res = INT_MAX, idx = 0;
		for (int i = 0; i < n; i += B) {
			int val = min(a + i, B);
			if (val < res) {
				res = val;
				idx = i;
			}
		}
		return { res, idx };
	}

	int argmin(int* a, int n) {

		auto [needle, base] = approx_argmin(a, n);
		int idx = find(a + base, B, needle);
		return base + idx;
	}
	*/

	//			Reverse
	void reverse1(int* a, int n) {
		for (int i = 0; i < n / 2; i++) {
			std::swap(a[i], a[n - i - 1]);
		}
	}// 1.8 GFLOPS

	const reg reversePermutation = _mm256_setr_epi32(
		7, 6, 5, 4, 3, 2, 1, 0
	);
	void reverse2(int* a, int n) {

		for (size_t i = 0; i < n / 2; i += 8)
		{
			reg x = _mm256_loadu_si256((reg*)&a[i]);
			reg y = _mm256_loadu_si256((reg*)&a[n - 8 - i]);

			_mm256_permutevar8x32_epi32(x, reversePermutation);
			_mm256_permutevar8x32_epi32(y, reversePermutation);

			_mm256_storeu_si256((reg*)&a[n - 8 - i], x);
			_mm256_storeu_si256((reg*)&a[i], y);

		}
	} // 7 GFLOPS (3.75x faster)

	//					FILTERING

	const int N = 16;
	int a[N], b[N];

	int filter1(int P)
	{
		int k = 0;
		for (size_t i = 0; i < N; i++)
		{
			if (a[i] < P)
				b[k++] = a[i];
		}
		return k;
	}// depending on P 0.7 - 1.5 GFLOPS


	//------------
	struct Precalc
	{
		alignas(64) int permutation[256][8];

		constexpr Precalc() : permutation{} {
			for (size_t m = 0; m < 256; m++)
			{
				int k = 0;
				for (size_t i = 0; i < 8; i++)
				{
					if (m >> i & 1) {
						permutation[m][k++] = i;
					}
				}
			}
		}
	};

	constexpr Precalc T;
	const int P = 23;
	const reg p = _mm256_set1_epi32(P);

	int filter2() {

		int k = 0;
		for (int i = 0; i < N; i += 8) {
			reg x = _mm256_load_si256((reg*)&a[i]);

			reg m = _mm256_cmpgt_epi32(p, x);
			int mask = _mm256_movemask_ps(psfrom(m));

			reg permutation = _mm256_load_si256((reg*)&T.permutation[mask]);

			x = _mm256_permutevar8x32_epi32(x, permutation);
			_mm256_storeu_si256((reg*)&b[k], x);

			k += std::popcount(static_cast<unsigned int>(mask));
		}
		return k;
	}// 3-7x faster than scalar one
	// above algorithm is much faster on AVX-512: it has a dedicated compress instruction
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