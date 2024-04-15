#pragma once
#include <immintrin.h>
#include <numeric>
#include <random>
//Non-Uniform Memory Architecture (NUMA): A Nearly Unfathomable Morass of Arcana - Fedor Pikus CppNow



// CPU Cache Effects - Sergey Slotin - Meeting C++ 2022 https://www.youtube.com/watch?v=mQWuX_KgH00
namespace memory1
{
	namespace ex1
	{
		const int N = 1024;
		int a[N];
		int enough_times_to_be_statistically_significant = 10000;

		void fun_read_write() {
			for (int t = 0; t < enough_times_to_be_statistically_significant; t++) {
				for (int i = 0; i < N; ++i) {
					a[i]++;
				}
			}
		}
		int s = 0;
		void fun_read_only() {
			for (int i = 0; i < N; ++i) {
				s += a[i];
			}
		}
		void fun_write_only() {
			for (int i = 0; i < N; ++i) {
				a[i] = 0;
			}// one more info: above loop may be replaced by memset by the compiler because we write same value.
		}
	}// flags -O3 -march=native -funroll-loops
	// 

	namespace ex2_bypass_caches_for_faster_write_only_operations
	{
		const int N = 1024;
		int a[N];
		void write_only_with_simd1() {
			//const __m256i zeroes = _mm256_setzero_si256();
			const __m256i zeroes = _mm256_set1_epi32(0);
			for (int i = 0; i < N; i += 8) {
				_mm256_store_si256((__m256i*) & a[i], zeroes); // fill 8 integers with zeroes
			}
		}
		/*
			8 elements/vector * 1 write/cycle * 2B cycles/second = 16GFLOPS(64GB/s)
			64 because each number written is 4 byte integer.=> 16 * 4 = 64 GB /s
		*/

		void write_only_with_simd_better_in_some_cases() {
			//const __m256i zeroes = _mm256_setzero_si256();
			const __m256i zeroes = _mm256_set1_epi32(0);
			for (int i = 0; i < N; i += 8) {
				_mm256_stream_si256((__m256i*) & a[i], zeroes); // fill 8 integers with zeroes
				// this is non-temporal store, it bypasses the cache and writes directly to memory.
			}
		}// when memory exceeds L3 cache perf will be >3x better.
		// but when data resides in L3 cache the throughput is >2x worse.
		// we can write special versions of memset and memcpy that use non-temporal stores.

		//Tip: Ram and cpu caches are different.
		//Tip: cache only the data you need.


	}


	namespace Latency1
	{
		const int N = 1 << 15;
		class LatencyTest {
			int p[N], q[N];
		public:
			LatencyTest() {
				std::iota(p, p + N, 0);
				std::shuffle(p, p + N, std::mt19937{std::random_device{}()});

				// make single cycle permutation out of it
				int k = p[N - 1];
				for (int i = 0; i < N; i++)
					k = q[k] = p[i];
				for (int i = 0; i < N; i++)
					k = q[k];
			}// "pointer-chasing" anti-pattern
		};
	}

	namespace Cache_Lines
	{
		const int N = 1 << 15;
		int a[N];
		class Cache_Line_Test
		{
		public:
			// tweakable parameter
			void test1(int D = 1) {
				for (int i = 0; i < N; i += D)
					a[i]++;
			}
			
		};
	}

	namespace Memory_Sharing
	{
		// lstopo on linux will show the numa nodes and memory
		// parallel taskset -c 0,1,2,3 ./app ::: {0..3}
		// will run 4 processes on first 4 cores.
		// numactl --interleave=all ./app
	}

	namespace Cache_Associativity
	{
		const int N = 2e6;
		int a[N];

		void test1() {
			for (int i = 0; i < N; i+=256)
				a[i]++;
		}// very slow
		void test2() {
			for (int i = 0; i < N; i += 257)
				a[i]++;
		}// 10x faster

		// for same reason binary_search is 20% slower on arrays of size 2^20 than 2^20+123
	}

}

