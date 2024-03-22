#pragma once
#include <benchmark/benchmark.h>
#include <vector>
#include <list>
#include "..\Helper\simplify.h"

/*

	Prefetching is a technique used to reduce the latency of memory access by fetching data into the cache before it is actually needed.

*******The important thing to remember.
The below examples are only true for some cases but not for others(func may take different abount of time and do other operations that
will cause all our efforts to be useless)
*******





Here's what each macro represents:
•	_MM_HINT_NTA: This stands for "Non-Temporal Aligned". It suggests that the data will be used once and will not be reused in the near future.
	The data will be fetched into the cache, but it will be the first to be evicted.
•	_MM_HINT_T0, _MM_HINT_T1, _MM_HINT_T2: These are temporal data prefetch hints. They suggest that the data will be reused in the near future.
	The difference between T0, T1, and T2 is the level of the cache where the data is loaded. T0 is the highest level (closest to the processor), and T2 is the lowest level (farthest from the processor).
•	_MM_HINT_ENTA: This is similar to _MM_HINT_NTA, but it's used for the second non-temporal prefetch instruction (prefetchw).
	This instruction is used to prefetch data into caches in anticipation of a write (rather than a read).
These hints are used to give the processor an idea of how the prefetched data will be used, allowing it to make better decisions about caching, which can lead to performance improvements.




Here are the meanings of the selected macros:
•	_MM_HINT_IT0: This hint indicates that the data being prefetched will be used for instruction fetches.
	It suggests that the data should be fetched into the instruction cache.
•	_MM_HINT_IT1: This hint indicates that the data being prefetched will be used for instruction fetches.
	It suggests that the data should be fetched into the instruction cache, with a higher priority than _MM_HINT_IT0.
*/

namespace fast_iteration
{
	static int sum{0};
	//_declspec(noinline)
	int func(int i)
	{
		sum += i;
		return sum;
	}



	void bm_list_iteration(benchmark::State& state) {
		std::list<int> list = random_list(int(state.range(0)));

		for (auto _ : state) {
			for (auto it = list.begin(); it != list.end(); ++it) {
				benchmark::DoNotOptimize(func(*it));
			}
		}
	}



	void bm_fast_list_iteration(benchmark::State& state) {
		std::list<int> list = random_list(int(state.range(0)));

		for (auto _ : state) {
			auto current = list.begin();
			auto next = std::next(current);
			if (next == list.end()) {
				func(*current);
				continue;
			}
			while (current != list.end()) {
				//_mm_prefetch((char*)&*next, _MM_HINT_T2);//46 microseconds instead of 32
				benchmark::DoNotOptimize(func(*current));
				//_mm_prefetch((char*)&*next, _MM_HINT_NTA);
				//_mm_prefetch((char*)&*next, _MM_HINT_T1);//39 makes worse
				//_mm_prefetch((char*)&*next, _MM_HINT_ENTA);// no big changes in this case
				_mm_prefetch((char*)&*next, _MM_HINT_T0);
				current = next++;
				// __builtin_prefetch(&*next, 0, 0); // in linux gcc clang
			}
		}
	}

			


	void bm_fast_list_iteration2(benchmark::State& state) {
		std::list<int> list = random_list(int(state.range(0)));

		for (auto _ : state) {
			auto current = list.begin();
			auto next = std::next(current);
			auto next1 = std::next(next);
			auto next2 = std::next(next1);
			for (int i = 0; i < list.size(); i += 2) {
				_mm_prefetch((char*)&*next, _MM_HINT_T0);
				benchmark::DoNotOptimize(func(*current));
				_mm_prefetch((char*)&*next1, _MM_HINT_T0);
				benchmark::DoNotOptimize(func(*next));
				current = next1++;
				next = next1++;
			}
		}
	}

	BENCHMARK(bm_list_iteration)->Range(1 << 14, 1 << 15)->Unit(benchmark::TimeUnit::kMicrosecond);
	BENCHMARK(bm_fast_list_iteration)->Range(1 << 14, 1 << 15)->Unit(benchmark::TimeUnit::kMicrosecond);
	BENCHMARK(bm_fast_list_iteration2)->Range(1 << 14, 1 << 15)->Unit(benchmark::TimeUnit::kMicrosecond);

}