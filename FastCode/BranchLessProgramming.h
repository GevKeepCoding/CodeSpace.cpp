#pragma once

#include <benchmark/benchmark.h>

namespace branch_less {

	void bl_branch_not_predicted(benchmark::State& state)
	{
		srand(1);
		auto n = state.range(0);
		std::vector<unsigned long> v1(n), v2(n);
		std::vector<int> c1(n);
		for (int64_t i = 0; i < n; ++i) {
			v1[i] = rand();
			v2[i] = rand();
			c1[i] = rand() & 0x1;
		}

		unsigned long* p1 = v1.data();
		unsigned long* p2 = v2.data();
		int* b1 = c1.data();
		for (auto _ : state) {
			unsigned long a1 = 0, a2 = 0;
			for (int64_t i = 0; i < n; ++i) {
				if (b1[i]) {
					a1 += p1[i];
				}
				else {
					a1 *= p2[i];
				}
			}
			benchmark::DoNotOptimize(a1);
			benchmark::DoNotOptimize(a2);
			benchmark::ClobberMemory();
		}
		state.SetItemsProcessed(n * state.iterations());
	}
	//BENCHMARK(branch_less::bl_branch_not_predicted)->Arg(1 << 22);

	void bl_branch_predicted(benchmark::State& state)
	{
		srand(1);
		auto n = state.range(0);
		std::vector<unsigned long> v1(n), v2(n);
		std::vector<int> c1(n);
		for (int64_t i = 0; i < n; ++i) {
			v1[i] = rand();
			v2[i] = rand();
			c1[i] = rand() >= 0;
		}

		unsigned long* p1 = v1.data();
		unsigned long* p2 = v2.data();
		int* b1 = c1.data();
		for (auto _ : state) {
			unsigned long a1 = 0, a2 = 0;
			for (int64_t i = 0; i < n; ++i) {
				if (b1[i]) {
					a1 += p1[i];
				}
				else {
					a1 *= p2[i];
				}
			}
			benchmark::DoNotOptimize(a1);
			benchmark::DoNotOptimize(a2);
			benchmark::ClobberMemory();
		}
		state.SetItemsProcessed(n * state.iterations());
	}
	//BENCHMARK(branch_less::bl_branch_predicted)->Arg(1 << 22);

	//////////////////////////////////////////////////////////////////////////
	void bl_branch_predicted1(benchmark::State& state)
	{
		srand(1);
		auto n = state.range(0);
		std::vector<unsigned long> v1(n), v2(n);
		std::vector<int> c1(n);
		for (int64_t i = 0; i < n; ++i) {
			v1[i] = rand();
			v2[i] = rand();
			if (i == 0) c1[i] = rand() >= 0;
			else c1[i] = !c1[i - 1];
		}

		unsigned long* p1 = v1.data();
		unsigned long* p2 = v2.data();
		int* b1 = c1.data();
		for (auto _ : state) {
			unsigned long a1 = 0, a2 = 0;
			for (int64_t i = 0; i < n; ++i) {
				if (b1[i]) {
					a1 += p1[i];
				}
				else {
					a1 *= p2[i];
				}
			}
			benchmark::DoNotOptimize(a1);
			benchmark::DoNotOptimize(a2);
			benchmark::ClobberMemory();
		}
		state.SetItemsProcessed(n * state.iterations());
	}
	//BENCHMARK(branch_less::bl_branch_predicted1)->Arg(1 << 22);

	////////////////////////////////////
	void bl_false_branch(benchmark::State& state)
	{
		srand(1);
		auto n = state.range(0);
		std::vector<unsigned long> v1(n), v2(n);
		std::vector<int> c1(n), c2(n);
		for (int64_t i = 0; i < n; ++i) {
			v1[i] = rand();
			v2[i] = rand();
			c1[i] = rand() & 0x1;
			c2[i] = !c1[i];
		}

		unsigned long* p1 = v1.data();
		unsigned long* p2 = v2.data();
		int* b1 = c1.data();
		int* b2 = c2.data();
		for (auto _ : state) {
			unsigned long a1 = 0, a2 = 0;
			for (int64_t i = 0; i < n; ++i) {
				if (b1[i] || b2[i]) {
					a1 += p1[i];
				}
				else {
					a1 *= p2[i];
				}
			}
			benchmark::DoNotOptimize(a1);
			benchmark::DoNotOptimize(a2);
			benchmark::ClobberMemory();
		}
		state.SetItemsProcessed(n * state.iterations());
	}
	//BENCHMARK(branch_less::bl_false_branch)->Arg(1 << 22);

	void bl_faster_false_branch(benchmark::State& state)
	{
		srand(1);
		auto n = state.range(0);
		std::vector<unsigned long> v1(n), v2(n);
		std::vector<int> c1(n), c2(n);
		for (int64_t i = 0; i < n; ++i) {
			v1[i] = rand();
			v2[i] = rand();
			c1[i] = rand() & 0x1;
			c2[i] = !c1[i];
		}

		unsigned long* p1 = v1.data();
		unsigned long* p2 = v2.data();
		int* b1 = c1.data();
		int* b2 = c2.data();
		for (auto _ : state) {
			unsigned long a1 = 0, a2 = 0;
			for (int64_t i = 0; i < n; ++i) {
				//if (bool(b1[i]) + bool(b2[i])) { // you will be surprised here
				//if (b1[i] | b2[i]) {
				if (b1[i] + b2[i]) {
					a1 += p1[i];
				}
				else {
					a1 *= p2[i];
				}
			}
			benchmark::DoNotOptimize(a1);
			benchmark::DoNotOptimize(a2);
			benchmark::ClobberMemory();
		}
		state.SetItemsProcessed(n * state.iterations());
	}
	//BENCHMARK(bl_faster_false_branch)->Arg(1 << 22);

	////////////////////////////////////////////////////////////////////////////

	// sum += cond ? expr1 : expr2;
	// term[2] = { expr2, expr1 };
	// sum += term[bool(cond)];
	// improves perf if 1) extra computations are small and 2) branch is poorly predicted
	void bl_slow_not_predicted_branch(benchmark::State& state)
	{
		srand(1);
		auto n = state.range(0);
		std::vector<unsigned long> v1(n), v2(n);
		std::vector<int> c1(n);
		for (int64_t i = 0; i < n; ++i) {
			v1[i] = rand();
			v2[i] = rand();
			c1[i] = rand() & 0x1;
		}

		unsigned long* p1 = v1.data();
		unsigned long* p2 = v2.data();
		int* b1 = c1.data();
		for (auto _ : state) {
			unsigned long a1 = 0, a2 = 0;
			for (int64_t i = 0; i < n; ++i) {
				if (b1[i]) {
					a1 += p1[i] - p2[i];
				}
				else {
					a1 += p1[i] * p2[i];
				}
			}
			benchmark::DoNotOptimize(a1);
			benchmark::DoNotOptimize(a2);
			benchmark::ClobberMemory();
		}
		state.SetItemsProcessed(n * state.iterations());
	}
	//BENCHMARK(bl_slow_not_predicted_branch)->Arg(1 << 22);

	void bl_predicted_version(benchmark::State& state)
	{
		srand(1);
		auto n = state.range(0);
		std::vector<unsigned long> v1(n), v2(n);
		std::vector<int> c1(n);
		for (int64_t i = 0; i < n; ++i) {
			v1[i] = rand();
			v2[i] = rand();
			c1[i] = rand() >= 0;
		}

		unsigned long* p1 = v1.data();
		unsigned long* p2 = v2.data();
		int* b1 = c1.data();
		for (auto _ : state) {
			unsigned long a1 = 0, a2 = 0;
			for (int64_t i = 0; i < n; ++i) {
				if (b1[i]) {
					a1 += p1[i] - p2[i];
				}
				else {
					a1 += p1[i] * p2[i];
				}
			}
			benchmark::DoNotOptimize(a1);
			benchmark::DoNotOptimize(a2);
			benchmark::ClobberMemory();
		}
		state.SetItemsProcessed(n * state.iterations());
	}
	//BENCHMARK(bl_predicted_version)->Arg(1 << 22);

	void bl_fast_branch_less_version(benchmark::State& state)
	{
		srand(1);
		auto n = state.range(0);
		std::vector<unsigned long> v1(n), v2(n);
		std::vector<int> c1(n);
		for (int64_t i = 0; i < n; ++i) {
			v1[i] = rand();
			v2[i] = rand();
			c1[i] = rand() & 0x1;
		}

		unsigned long* p1 = v1.data();
		unsigned long* p2 = v2.data();
		int* b1 = c1.data();
		for (auto _ : state) {
			unsigned long a1 = 0, a2 = 0;
			for (int64_t i = 0; i < n; ++i) {
				unsigned long s1[2] = { 0, p1[i] - p2[i] };
				unsigned long s2[2] = { p1[i] * p2[i],0 };
				a1 += s1[bool(b1[i])];
				a2 += s2[bool(b1[i])];
			}
			benchmark::DoNotOptimize(a1);
			benchmark::DoNotOptimize(a2);
			benchmark::ClobberMemory();
		}
		state.SetItemsProcessed(n * state.iterations());
	}
	//BENCHMARK(bl_fast_branch_less_version)->Arg(1 << 22);
	//////////////////////////////////////////////////////////////////////////
	//void __attribute__((noinline)) f(bool b, unsigned long x, unsigned long& s) {
	void __declspec(noinline) f1(bool b, unsigned long x, unsigned long& s) {
			if (b) s += x;
	}
	void bl_branch(benchmark::State& s) {
		srand(1);
		auto n = s.range(0);
		std::vector<unsigned long> d(n);
		std::vector<int> b(n);
		for (int64_t i = 0; i < n; ++i) {
			b[i] = rand() & 0x1;
		}
		const unsigned long* const p1 = d.data();
		const bool* b1 = (bool*)b.data();
		for (auto _ : s) {
			unsigned long a1 = 0;
			for (int64_t i = 0; i < n; ++i) {
				f1(b1[i], p1[i], a1);
			}
			benchmark::DoNotOptimize(a1);
			benchmark::ClobberMemory();
		}
		s.SetItemsProcessed(n * s.iterations());
	}
	//BENCHMARK(bl_branch)->Arg(1 << 22);

	void __declspec(noinline) f2(bool b, unsigned long x, unsigned long& s) {
		s += b * x;
	}

	void bl_branchless(benchmark::State& s) {
		srand(1);
		auto n = s.range(0);
		std::vector<unsigned long> d(n);
		std::vector<int> b(n);

		for (int64_t i = 0; i < n; ++i) {
			b[i] = rand() & 0x1;
		}

		const unsigned long* const p1 = d.data();
		const bool* b1 = (bool*)b.data();
		for (auto _ : s) {
			unsigned long a1 = 0;
			for (int64_t i = 0; i < n; ++i) {
				f2(b1[i], p1[i], a1);
			}
			benchmark::DoNotOptimize(a1);
			benchmark::ClobberMemory();
		}
		s.SetItemsProcessed(n * s.iterations());
	}
	//BENCHMARK(bl_branchless)->Arg(1 << 22); // not faster than perfectly predicted branch
	// only mispredicated branch will become faster.
	///////////////////////////////////////////////////////////////////////


	}






