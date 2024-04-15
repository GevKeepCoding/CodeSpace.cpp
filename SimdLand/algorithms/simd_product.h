#pragma once

#include "../Helpers/simplify.h"

#include "benchmark/benchmark.h"


namespace simd_vector_products {

	inline float prod8(const float* a, const float* b)
	{
		union { float r[8]; __m256 rv; };
		rv = _mm256_dp_ps(_mm256_load_ps(a),
			_mm256_load_ps(b), 0xf1);

		return r[0] + r[4];
	}
	inline float prod8_bypass_caches(const float* a, const float* b)
	{
		union { float r[8]; __m256 rv; };
		rv = _mm256_dp_ps(reinterpret_cast<__m256>(_mm256_stream_load_si256((const __m256i*)a)),
			reinterpret_cast<__m256>(_mm256_stream_load_si256((const __m256i*)b)), 0xf1);

		return r[0] + r[4];
	}

	void mem_read(void* memory, size_t size) {
		void* const end = static_cast<char*>(memory) + size;
		__m256i* const p0 = static_cast<__m256i*>(memory);
		__m256i* const p1 = static_cast<__m256i*>(end);
		for (const volatile __m256i* p = p0; p < p1; ) {
			REPEAT(sink = *p++);
		}
	}

	namespace bm {

		template<class T>
		void make_random_data(size_t N, T& a, unsigned int seed) {
			srand(seed);
			unsigned int s1 = seed, s2 = seed;
			for (size_t i = 0; i < N; ++i) {
				a[i] = rand_r(&s1) * rand_r(&s2);
			}
		}

		void bm_mv_prod(benchmark::State& state) {
			const size_t N = state.range(0);
			float* m = allocate(N * N);
			float* v = allocate(N);
			float* r = allocate(N);
			while (state.KeepRunning()) {
				mv_prod(N, &m[0], &v[0], &r[0]);
			}

			free(m);
			free(v);
			free(r);

			state.setItemsProcessed(N * N * state.iterations());
			state.setBytesProcessed(sizeof(float) * N * (N + 2) * state.iterations());
			char buf[1024];
			snprintf(buf, sizeof(buf), "Memory: %g+%g+%g (K)", sizeof(float) * N * N / 1024., sizeof(float) * N / 1024., sizeof(float) * N / 1024.);
			state.setLabel(buf);

		}

		BENCHMARK(bm_mv_prod)->RangeMultiplier(2)->Range(1 << 8, 1 << 10);

	}

}