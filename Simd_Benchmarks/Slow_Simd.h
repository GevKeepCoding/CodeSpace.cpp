#pragma once
#include <benchmark/benchmark.h>
#include <immintrin.h>

namespace slow_simd_examples
{
	constexpr int n = 2048;
	double a[n], b[n], c[n];

	__declspec(noinline)
		void addv1(double* a, double* b, double* c, int n)
	{
		for (int i = 0; i < n; ++i)
			c[i] = a[i] + b[i];
	}

	__declspec(noinline)
		void addv2(double* __restrict a, double* __restrict b, double* __restrict c, int n)
	{
		for (int i = 0; i < n; ++i)
			c[i] = a[i] + b[i];
	}

	// I have applied /ARCH:AVX512 flag to the compiler
	__declspec(noinline)
		void slow_simd(double* __restrict a, double* __restrict b, double* __restrict c, int n)
	{
		for (int i = 0; i + 7 < n; i += 8) {
			auto aa = _mm512_loadu_pd(a + i);
			auto bb = _mm512_loadu_pd(b + i);
			aa = _mm512_add_pd(aa, bb);
			_mm512_storeu_pd(c + i, aa);
		}
	}

	constexpr auto size = sizeof(double);

	void BM_addv1(benchmark::State& state)
	{
		for (auto _ : state) {
			addv1(a, b, c, n);
			benchmark::DoNotOptimize(c);
			benchmark::ClobberMemory();
		}
		state.SetItemsProcessed(n * state.iterations());
	}

	void BM_addv2(benchmark::State& state)
	{
		for (auto _ : state) {
			addv2(a, b, c, n);
			benchmark::DoNotOptimize(c);
			benchmark::ClobberMemory();
		}
		state.SetItemsProcessed(n * state.iterations());
	}


	void BM_slow_simd(benchmark::State& state)
	{
		for (auto _ : state) {
			slow_simd(a, b, c, n);
			benchmark::DoNotOptimize(c);
			benchmark::ClobberMemory();
		}
		state.SetItemsProcessed(n * state.iterations());
	}

}

//BENCHMARK(slow_simd_examples::BM_addv2);// ->Arg(1 << 11);//->Unit(benchmark::kMicrosecond);
//BENCHMARK(slow_simd_examples::BM_addv1);// ->Arg(1 << 11);//->Unit(benchmark::kMicrosecond);
//BENCHMARK(slow_simd_examples::BM_slow_simd);// ->Arg(1 << 11);// ->Unit(benchmark::kMillisecond);