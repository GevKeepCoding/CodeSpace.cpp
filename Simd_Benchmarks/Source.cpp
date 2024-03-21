//#pragma comment(lib, "benchmark.lib")
//#pragma comment(lib, "benchmark_main.lib")
#pragma comment(lib, "shlwapi.lib")
#include <benchmark/benchmark.h>
//#include "Slow_Simd.h"
//#include "Vectorized_transform_reduce_perf.h"

import std;


using reg = __m256i;

int hsum_fastest(reg x)//horizontal sam
{
	reg t = _mm256_hadd_epi32(x, x);
	t = _mm256_hadd_epi32(t, t);
	return _mm256_extract_epi32(t, 0) + _mm256_extract_epi32(t, 4);
}


int hsum(reg x)//horizontal sam
{
	int t[8], s = 0;
	_mm256_storeu_si256((reg*)t, x);
	for (int i = 0; i < 8; ++i) {
		s += t[i];
	}
	return s;
}

void hsum_bench(benchmark::State& state) {
	reg x = _mm256_setr_epi32(1, 2, 3, 4, 5, 6, 7, 8);
	for (auto _ : state) {
		benchmark::DoNotOptimize(hsum(x));
	}
}

void hsum2_bench(benchmark::State& state) {
	reg x = _mm256_setr_epi32(1, 2, 3, 4, 5, 6, 7, 8);
	for (auto _ : state) {
		benchmark::DoNotOptimize(hsum_fastest(x));
	}
}

BENCHMARK(hsum_bench);
BENCHMARK(hsum2_bench);









BENCHMARK_MAIN();