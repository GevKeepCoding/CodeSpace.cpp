#pragma once
#include <benchmark/benchmark.h>
#include <random>
#include <vector>
#include <numeric>


//http://github.com/coffeebeforearch
// Parallel C++: SIMD Intrinsics
//https://www.youtube.com/watch?v=PuOGJ527k5E
namespace PARALLEL_CPP_SIMD_INTRINSICS
{
	static void dot_product_normal(benchmark::State& s) {
		// Create random number generator
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution d(0.0f, 1.0f);

		// Create vectors of random numbers
		const int num_elements = 1 << 15;
		std::vector<float> v1, v2;
		std::ranges::generate_n(std::back_inserter(v1), num_elements, [&]() { return d(mt); });
		std::ranges::generate_n(std::back_inserter(v2), num_elements, [&]() { return d(mt); });


		// Perform dot product
		float* sink = new float;
		for (auto _ : s) {
			*sink = std::transform_reduce(v1.begin(), v1.end(), v2.begin(), 0.0f);
		}
	}

	using reg = __m256;
	float dot_product(const reg* v1, const reg* v2,
					  size_t num_packed_elements) {

		auto tmp = 0.0f;
		for (size_t i = 0; i < num_packed_elements; i++)
		{
			float unpacked[8];
			reg result;

			// Our Dot product intrinsic
			result = _mm256_dp_ps(v1[i], v2[i], 0xF1);

			// Extract __256 into array of floats
			_mm256_storeu_ps(unpacked, result);

			tmp += unpacked[0] + unpacked[4];
		}
		return tmp;
	}

	static void dot_product_bench(benchmark::State& s) {
		// Get the size of the vector
		const size_t num_elements = 1 << 15;
		const size_t elements_per_reg = 8;
		const size_t num_packed_elements = num_elements / elements_per_reg;

		// Create random number generator
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution dist(0.0f, 1.0f);

		// allocate __m256 aligned 32Byte
		auto v1 = static_cast<__m256*>(
			_aligned_malloc(num_packed_elements * sizeof(__m256), 32));

		auto v2 = static_cast<__m256*>(
			_aligned_malloc(num_packed_elements * sizeof(__m256), 32));

		// Create random numbers for v1 and v2
		for (size_t i = 0; i < num_packed_elements; i++) {
			v1[i] = _mm256_set_ps(dist(mt), dist(mt), dist(mt), dist(mt),
								  dist(mt), dist(mt), dist(mt), dist(mt));
			v2[i] = _mm256_set_ps(dist(mt), dist(mt), dist(mt), dist(mt),
								  dist(mt), dist(mt), dist(mt), dist(mt));
		}

		// Perform dot product
		float* result = new float;
		for (auto _ : s) {
			*result = dot_product(v1, v2, num_packed_elements);
		}

		// Free memory when we're done
		_aligned_free(v1);
		_aligned_free(v2);
	}
	
	//BENCHMARK(dot_product_bench)->Unit(benchmark::kMicrosecond);
	//BENCHMARK(dot_product_normal)->Unit(benchmark::kMicrosecond);

}

