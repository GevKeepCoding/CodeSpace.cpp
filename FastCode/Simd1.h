#pragma once
#include <benchmark/benchmark.h>
//# this is heuristically generated, and may not be correct
//find_package(benchmark CONFIG REQUIRED)
//target_link_libraries(main PRIVATE benchmark::benchmark benchmark::benchmark_main)

int a[2000], b[2000], c[2000];



void BM_slowLoop(benchmark::State& state)
{
	for (auto _ : state) {
		for (int i = 0; i < 1000; ++i) {
			a[i] += b[i];
			b[i + 1] += c[i];
		}
	}
}
void BM_fastLoop(benchmark::State& state)
{
	for (auto _ : state) {
		a[0] = b[0];
		for (int i = 0; i < 999; ++i) {
			b[i + 1] += c[i];
			a[i + 1] += b[i + 1];
		}
		b[999] += c[998];
	}
}


//BENCHMARK(BM_slowLoop);//->Unit(benchmark::kMicrosecond);
//
//BENCHMARK(BM_fastLoop);// ->Unit(benchmark::kMillisecond);

