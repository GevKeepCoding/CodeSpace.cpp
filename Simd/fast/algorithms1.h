#pragma once

#include <benchmark/benchmark.h>

import std;

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