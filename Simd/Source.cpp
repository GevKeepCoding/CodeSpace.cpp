//BENCHMARK_STATIC_DEFINE;

#pragma comment(lib, "shlwapi.lib")
#include <benchmark/benchmark.h>

#include <immintrin.h>
#include "random_data.h"
#include "algorithms01.h"




//Making use of SIMD Vectorisation to Improve Code Performance
// https://www.youtube.com/watch?v=62_TLN-wk4s
// James Willis (SciNet)
//https://www.computeontario.ca/training-colloquia


__m256i mmData = _mm256_set1_epi32(1);
__m256i mmMax = _mm256_set1_epi32(0);

bool test = [&]() {
	

	auto res = _mm256_cmpgt_epi32(mmData, mmMax);
	
	
	return mysimd_max_element::max_of_8_in32_v2(res);


}();
























//
//int s = 0;
//
//__declspec(noinline)
//int sum(const std::vector<unsigned long>& v) {
//	for (int i = 0; i < v.size(); ++i)
//		s += v[i];
//	return s;
//}
//
//
//void avx2Perf(benchmark::State& state)
//{
//	auto N = state.range(0);
//	srand(1);
//	std::vector<unsigned long> v1(N);
//	for (int64_t i = 0; i < N; ++i) {
//		v1[i] = rand();
//	}
//
//	for (auto _ : state)
//		s += sum(v1);
//
//	state.SetItemsProcessed(N * state.iterations());
//	state.SetBytesProcessed(N * sizeof(long) * state.iterations());
//	state.counters["Items"] = N;
//}

//BENCHMARK(avx2Perf)->Arg(1 << 10);//->Unit(benchmark::kSecond)


















































//struct FooUpdateInOut {
//	float velocity[2];
//	float foo;
//};
//
//
//struct FooUpdateIn
//{
//	float velocity[2];
//	float foo;
//};
//
//struct FooUpdateOut {
//	float foo;
//};
//
//__declspec(noinline)
//void UpdateFoo(const FooUpdateIn* in, FooUpdateOut* out, int count, float f)
//{
//	for (int i = 0; i < count; ++i)
//	{
//		float mag = sqrtf(in[i].velocity[0] * in[i].velocity[0] + in[i].velocity[1] * in[i].velocity[1]);
//		out[i].foo = in[i].foo + mag * f;
//	}
//}
//
//__declspec(noinline)
//void UpdateFoo2(FooUpdateInOut* a, int count, float f)
//{
//	for (int i = 0; i < count; ++i)
//	{
//		float mag = sqrtf(a[i].velocity[0] * a[i].velocity[0] + a[i].velocity[1] * a[i].velocity[1]);
//		a[i].foo += mag * f;
//	}
//}
//
//static void BM_UpdateFoo2(benchmark::State& state) {
//	FooUpdateInOut* a = new FooUpdateInOut[state.range(0)];
//	for (auto _ : state) {
//		UpdateFoo2(a, (int)state.range(0), 0.1f);
//		benchmark::DoNotOptimize(a);
//		benchmark::ClobberMemory();
//	}
//	state.SetItemsProcessed(state.range(0) * sizeof(a[0]) * state.iterations());
//	delete[] a;
//}
//
//static void BM_UpdateFoo(benchmark::State& state) {
//	FooUpdateIn* in = new FooUpdateIn[state.range(0)];
//	FooUpdateOut* out = new FooUpdateOut[state.range(0)];
//	for (auto _ : state) {
//		UpdateFoo(in, out, (int)state.range(0), 0.1f);
//		benchmark::DoNotOptimize(in);
//		benchmark::DoNotOptimize(out);
//		benchmark::ClobberMemory();
//	}
//	state.SetItemsProcessed(state.range(0) * (sizeof(in[0]) + sizeof(out[0])) * state.iterations());
//
//	delete[] in;
//	delete[] out;
//}
//
////16k = 1 << 14;
//BENCHMARK(BM_UpdateFoo)->Range(1 << 20, 1 << 21);
////BENCHMARK(BM_UpdateFoo)->Range(1 << 14, 1 << 15);
//
//BENCHMARK(BM_UpdateFoo2)->Range(1 << 20, 1 << 21);
//


//_EXPORT_STD using nano = ratio<1, 1000000000>;
//_EXPORT_STD using micro = ratio<1, 1000000>;
//_EXPORT_STD using milli = ratio<1, 1000>;



BENCHMARK_MAIN();