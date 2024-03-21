#pragma once


#include <benchmark/benchmark.h>

unsigned long a1 = 0, a2 = 0;

struct MyStruct
{
	int isleap04 = 0;
	int isleap01 = 0;
	int* p = new int(3);
	const std::vector<int> vec;
	__declspec(noinline) ~MyStruct();
};
MyStruct s;

MyStruct::~MyStruct() {
	delete p;
}
namespace Better_Branch_Predictions01 // Very different results with different compilers/versions.
{
	int N = 1 << 22;
	std::vector<int> v1 = []() -> std::vector<int>
	{
		std::vector<int> result(N);
		srand(uint32_t(time(NULL)));
		for (int64_t i = 0; i < N; ++i) {
			result[i] = rand();
		}
		return result;
	}();
	int* p1 = v1.data();





	// y % 4 == 0 && (y % 100 != 0 || y % 400 == 0 )
	__declspec(noinline)
	bool IsLeapYear04(int y) {
		int d = y % 100 != 0 ? 4 : 16;
		return (y & (d - 1)) == 0; // 1% 
	}
	// 3x faster than not predicted.
	void BM_IsLeapYear_04(benchmark::State& state)
	{
		for (auto _ : state) {
			a1 = 0;
			for (int i = 0; i < N; ++i) {
				if (IsLeapYear04(p1[i])) {
					a1 += 1;
				}
				else
				{
					a2 += 1;
				}
			}
			s.isleap04 = a1;
			benchmark::DoNotOptimize(a1);
			benchmark::DoNotOptimize(a2);
			benchmark::ClobberMemory();
		}
		state.SetItemsProcessed(N * state.iterations());
	}
	//BENCHMARK(BM_IsLeapYear_04)->Arg(1 << 22);



	// y % 4 == 0 && (y % 100 != 0 || y % 400 == 0 )
	__declspec(noinline)
	bool IsLeapYear01(int y) {
		if (y % 4 != 0) //75 %
			return false;
		// below will be other 25%
		if (y % 100 != 0)
			return true;
		return y % 400 == 0;
	}


	void BM_IsLeapYear_branch_not_predicted(benchmark::State& state)
	{
		for (auto _ : state) {
			a1 = 0;
			for (int i = 0; i < N; ++i) {
				if (IsLeapYear01(p1[i])) {
					a1 += 1;
				}
				else
				{
					a2 += 1;
				}
			}
			s.isleap01 = a1;
			benchmark::DoNotOptimize(a1);
			benchmark::DoNotOptimize(a2);
			benchmark::ClobberMemory();
		}
		state.SetItemsProcessed(N * state.iterations());
	}
	//BENCHMARK(BM_IsLeapYear_branch_not_predicted)->Arg(1 << 22);



	// y % 4 == 0 && (y % 100 != 0 || y % 400 == 0 )
	//__declspec(noinline)
	bool IsLeapYear02(int y) {
		if (y % 100 != 0)
			return y % 4 == 0;  // 99%

		return y % 400 == 0; // 1% 
	}

	// 3x faster than not predicted.
	void BM_IsLeapYear_branch_predicted(benchmark::State& state)
	{
		for (auto _ : state) {
			for (int64_t i = 0; i < N; ++i) {
				if (IsLeapYear02(p1[i])) {
					a1 += 1;
				}
				else
				{
					a2 += 1;
				}
			}
			benchmark::DoNotOptimize(a1);
			benchmark::DoNotOptimize(a2);
			benchmark::ClobberMemory();
		}
		state.SetItemsProcessed(N * state.iterations());
	}
	//BENCHMARK(BM_IsLeapYear_branch_predicted)->Arg(1 << 22);

	// y % 4 == 0 && (y % 100 != 0 || y % 400 == 0 )
	//__declspec(noinline)
	bool IsLeapYear03(int y) {
		if (y % 100 != 0)
			return y % 4 == 0;  // 99%

		//return y % 16 == 0; // 1% 
		return (y & 15) == 0; // 1% 
	}
	// 3x faster than not predicted.
	void BM_IsLeapYear_branch_predicted_AndMore(benchmark::State& state)
	{
		for (auto _ : state) {
			for (int64_t i = 0; i < N; ++i) {
				if (IsLeapYear03(p1[i])) {
					a1 += 1;
				}
				else
				{
					a2 += 1;
				}
			}
			benchmark::DoNotOptimize(a1);
			benchmark::DoNotOptimize(a2);
			benchmark::ClobberMemory();
		}
		state.SetItemsProcessed(N * state.iterations());
	}
	//BENCHMARK(BM_IsLeapYear_branch_predicted_AndMore)->Arg(1 << 22);


}