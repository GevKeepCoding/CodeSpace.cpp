#include <benchmark/benchmark.h>
#include "../../SimdLand/algorithms/simd_algorithm1.h"



using namespace fast_addition_of_8_integers;

void hsum_bench(benchmark::State& state, auto&&... func) {
	reg x = _mm256_setr_epi32(1, 2, 3, 4, 5, 6, 7, 8);
	for (auto _ : state) {
		benchmark::DoNotOptimize(func(x)...);
	}
}

BENCHMARK_CAPTURE(hsum_bench, hsum3b, sum_8_integers);
//BENCHMARK_CAPTURE(hsum_bench, hsum1b, hsum1);
//BENCHMARK_CAPTURE(hsum_bench, hsum2b, hsum2);
//BENCHMARK_CAPTURE(hsum_bench, hsum2b, hsum_copy);

