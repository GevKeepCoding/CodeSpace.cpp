#pragma once
#include <benchmark/benchmark.h>

static const int N = 128 * 1024;
std::vector<unsigned long> v1(N);
bool start = [] {
	for (int64_t i = 0; i < N; ++i) {
		v1[i] = rand();
	}
	return true;
	}();



void ClearArray2(benchmark::State& state)
{
	srand(1);
	//auto N = state.range(0);
	//std::vector<unsigned long> v1(N);
	//for (int64_t i = 0; i < N; ++i) {
	//	v1[i] = rand();
	//}

	unsigned long* p1 = v1.data();
	for (auto _ : state) {
		auto zero = _mm_setzero_si128();
		for (int i = 0; i < N; i += 4) {

			__m128i* ptr = reinterpret_cast<__m128i*>(&p1[i]);
			_mm_storeu_si128(ptr, zero);
		}
		benchmark::ClobberMemory();
	}
	state.SetItemsProcessed(N * state.iterations());
}
//BENCHMARK(ClearArray2)->Range(1 << 10, N)->ReportAggregatesOnly();
BENCHMARK(ClearArray2)->Arg(N);

void ClearArray1(benchmark::State& state)
{
	srand(1);
	auto N = state.range(0);


	unsigned long* p1 = v1.data();
	for (auto _ : state) {
		for (int i = 0; i < N; ++i) {
			p1[i] = 0;
		}
		benchmark::ClobberMemory();
	}
	state.SetItemsProcessed(N * state.iterations());
}
BENCHMARK(ClearArray1)->Arg(N);



