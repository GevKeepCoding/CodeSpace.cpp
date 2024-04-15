//#pragma comment(lib, "benchmark.lib")
//#pragma comment(lib, "benchmark_main.lib")
#pragma comment(lib, "shlwapi.lib")
#include <benchmark/benchmark.h>
#include <iostream>
//#include "Slow_Simd.h"
//#include "Vectorized_transform_reduce_perf.h"
//#include   "Matrix_multiply_optimization_tricks.h"

//import std;
using std::vector;


//
//bool test = []() {
//
//	vector<int32_t> data{1,2,3,4,5,6,7,8,9};
//	auto maxValue = FindMaxValue(data);
//
//
//	return true;
//}();

_declspec(noinline) int read_value(int* p)
{
	return *p;
}

_declspec(noinline)
void print_zregv2(__m512i r)
{
	int32_t* p = (int32_t*)&r;
	for (int i = 0; i < 16; i++)
	{
		benchmark::DoNotOptimize(read_value(&p[i]));
		//std::print(std::cout, "{} ", p[i]);
	}

}

static void BM_print_zreg(benchmark::State& state) {
    __m512i r = _mm512_set1_epi32(42);
    for (auto _ : state) {
        print_zreg(r);
    }
}

static void BM_print_zregv2(benchmark::State& state) {
    __m512i r = _mm512_set1_epi32(42);
    for (auto _ : state) {
        print_zregv2(r);
		//benchmark::DoNotOptimize(r);
    }
}

BENCHMARK(BM_print_zregv2);
BENCHMARK(BM_print_zreg);

BENCHMARK_MAIN();