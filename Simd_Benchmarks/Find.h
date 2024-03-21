#pragma once

//#include <immintrin.h>
#include <benchmark/benchmark.h>
import std;

using reg = __m256i;

int find3(int* a, int n, int needle) {
	reg x = _mm256_set1_epi32(needle);

	for (int i = 0; i < n; i += 16) {
		reg y1 = _mm256_loadu_si256((__m256i*) & a[i]);
		reg y2 = _mm256_loadu_si256((__m256i*) & a[i + 8]);
		reg m1 = _mm256_cmpeq_epi32(x, y1);
		reg m2 = _mm256_cmpeq_epi32(x, y2);
		if (!_mm256_testz_si256(m1, m1)) {
			return i + std::countr_zero(unsigned(_mm256_movemask_ps(_mm256_castsi256_ps(m1))));
		}
		if (!_mm256_testz_si256(m2, m2)) {
			return i + 8 + std::countr_zero(unsigned(_mm256_movemask_ps(_mm256_castsi256_ps(m2))));
		}
	}
	return -1;
}


int find4(int* a, int n, int needle) {
	reg x = _mm256_set1_epi32(needle);

	for (int i = 0; i < n; i += 16) {
		reg y1 = _mm256_loadu_si256((__m256i*) & a[i]);
		reg y2 = _mm256_loadu_si256((__m256i*) & a[i + 8]);
		reg m1 = _mm256_cmpeq_epi32(x, y1);
		reg m2 = _mm256_cmpeq_epi32(x, y2);

		reg m = _mm256_or_si256(m1, m2);
		if (!_mm256_testz_si256(m, m)) {

			if (!_mm256_testz_si256(m1, m1)) {
				return i + std::countr_zero(unsigned(_mm256_movemask_ps(_mm256_castsi256_ps(m1))));
			}
			if (!_mm256_testz_si256(m2, m2)) {
				return i + 8 + std::countr_zero(unsigned(_mm256_movemask_ps(_mm256_castsi256_ps(m2))));
			}
		}
	}
	return -1;
}

int* Init(int n) {
	int* a = new int[n];
	for (int i = 0; i < n; i++) {
		a[i] = i;
	}
	return a;
}

void find3_perf(benchmark::State& state)
{
	int n = int(state.range(0));
	int* a = Init(n);

	int needle = n - 1;

	for (auto _ : state)
	{
		benchmark::DoNotOptimize(find3(a, n, needle));
	}
	delete[] a;
	state.SetItemsProcessed(n * state.iterations());
	//state.SetComplexityN(n);
}

void find4_perf(benchmark::State& state)
{
	int n = int(state.range(0));
	int* a = Init(n);

	int needle = n - 1;

	for (auto _ : state)
	{
		benchmark::DoNotOptimize(find4(a, n, needle));
	}
	delete[] a;
	state.SetItemsProcessed(n * state.iterations());
	//state.SetComplexityN(n);
}


BENCHMARK(find3_perf)->RangeMultiplier(2)->Range(1 << 12, 1 << 15);
BENCHMARK(find4_perf)->RangeMultiplier(2)->Range(1 << 12, 1 << 15);

