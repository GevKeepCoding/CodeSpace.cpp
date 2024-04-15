#pragma once

#pragma comment(lib, "shlwapi.lib")
#include <benchmark/benchmark.h>
#include "..\Helper\simplify.h"
#include <numeric>


namespace fast_addition_of_8_integers
{

	using reg = __m256i;

	int hsum1(reg x)
	{
		int t[8], s = 0;
		_mm256_storeu_si256((reg*)t, x);
		for (int i = 0; i < 8; ++i) {
			s += t[i];
		}
		return s;
	}


	int hsum2(reg x)
	{
		int s = 0;
		for (int i = 0; i < 8; ++i) {
			s += x.m256i_i32[i];
		}
		return s;
	}

	int hsum_fastest(reg x)
	{
		reg t = _mm256_hadd_epi32(x, x);
		t = _mm256_hadd_epi32(t, t);
		return _mm256_extract_epi32(t, 0) + _mm256_extract_epi32(t, 4);
	}

	void hsum1_bench(benchmark::State& state) {
		reg x = _mm256_setr_epi32(1, 2, 3, 4, 5, 6, 7, 8);
		for (auto _ : state) {
			benchmark::DoNotOptimize(hsum1(x));
		}
	}

	void hsum2_bench(benchmark::State& state) {
		reg x = _mm256_setr_epi32(1, 2, 3, 4, 5, 6, 7, 8);
		for (auto _ : state) {
			benchmark::DoNotOptimize(hsum2(x));
		}
	}

	void hsum_fastest_bench(benchmark::State& state) {
		reg x = _mm256_setr_epi32(1, 2, 3, 4, 5, 6, 7, 8);
		for (auto _ : state) {
			benchmark::DoNotOptimize(hsum_fastest(x));
		}
	}

	/*BENCHMARK(hsum1_bench);
	BENCHMARK(hsum2_bench);
	BENCHMARK(hsum_fastest_bench);*/



	template<class...Args>
	void hsum_bench(benchmark::State& state, Args&&... func) {
		reg x = _mm256_setr_epi32(1, 2, 3, 4, 5, 6, 7, 8);
		for (auto _ : state) {
			benchmark::DoNotOptimize(func(x)...);
		}
	}

	//BENCHMARK_CAPTURE(hsum_bench, hsum1b, hsum1);
	//BENCHMARK_CAPTURE(hsum_bench, hsum2b, hsum2);
	//BENCHMARK_CAPTURE(hsum_bench, hsum3b, hsum_fastest);
}

//SIMD и ручная векторизация (доп. семинар для первого курса по языку C и алгоритмам)
// https://github.com/tilir/c-graduate/blob/master/simd/simd.h
namespace find_simd
{
namespace v1
{
int find(const int* a, int n, int x)
{
	int i;
	for (i = 0; i < n; i++)
		if (a[i] == x)
			return i;
	return -1;
}


int find_index_of_first_equal_zvalues(ri5 a, ri5 b)
{
	unsigned short mask = _mm512_cmp_epi32_mask(a, b, _MM_CMPINT_EQ);
	if (mask != 0)
	{
		// i need to return first non zero position in mask
		return std::countr_zero(mask);
	}
	return -1;
}


int find_simd(const int* a, int n, int x)
{
	int i;
	ri5 needle = ::set_zvalues(x);
	int mainsz = (n / 16) * 16;

	for (i = 0; i < mainsz; i += 16)
	{
		ri5 undertest = ::load_zvalue_unaligned(a + i);
		int m = find_index_of_first_equal_zvalues(needle, undertest);
		if (m != -1)
			return i + m;
	}

	for (i = mainsz; i < n; i++)
		if (a[i] == x)
			return i;
	return -1;
}


static void find_simd_bench(benchmark::State& state) {
	std::vector<int> v(1000000);
	std::iota(v.begin(), v.end(), 0);
	int needle = 999999;
	for (auto _ : state) {
		benchmark::DoNotOptimize(find_simd(v.data(), int(v.size()), needle));
	}
}

static void find_bench(benchmark::State& state) {
	std::vector<int> v(1000000);
	std::iota(v.begin(), v.end(), 0);
	int needle = 999999;
	for (auto _ : state) {
		benchmark::DoNotOptimize(find(v.data(), int(v.size()), needle));
	}
}

static void find_stl_bench(benchmark::State& state) {
	std::vector<int> v(1000000);
	std::iota(v.begin(), v.end(), 0);
	int needle = 999999;
	for (auto _ : state) {
		benchmark::DoNotOptimize(std::find(v.begin(), v.end(), needle));
	}
}

//BENCHMARK(find_simd_bench); // fastest one: todo add loop unrolling (look to Stolin)
//BENCHMARK(find_bench);
//BENCHMARK(find_stl_bench);
//BENCHMARK_MAIN();
}





}





