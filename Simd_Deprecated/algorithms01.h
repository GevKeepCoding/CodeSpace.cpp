#pragma once

#include <immintrin.h>
#include <vector>

//SIMD optimization tips and tricks by Oleksandr Palamarchuk(ENG) : https://www.youtube.com/watch?v=AFtpRG500TU
namespace mysimd_max_element
{
	int32_t max1(const std::vector<int32_t>& data)
	{
		const auto arrayLength = data.size();
		auto dataPtr = data.data();
		auto index = 0;
		auto mmMax = _mm256_set1_epi32(std::numeric_limits<int32_t>::lowest());
		for (; index + 7 < arrayLength; index += 8) {
			const auto mmData = _mm256_lddqu_si256(reinterpret_cast<const __m256i*>(dataPtr + index));
			mmMax = _mm256_max_epi32(mmMax, mmData);
		}
		auto maxValues = _mm256_extracti128_si256(mmMax, 1);
		maxValues = _mm_max_epi32(maxValues, _mm256_castsi256_si128(mmMax));
		auto b = _mm_srli_si128(maxValues, 8);
		maxValues = _mm_max_epi32(maxValues, b);
		auto maxValue1 = _mm_cvtsi128_si32(maxValues);
		auto maxValue2 = _mm_extract_epi32(maxValues, 1);
		auto maxValue = std::max(maxValue1, maxValue2);

		for (; index < arrayLength; ++index) {
			maxValue = std::max(maxValue, dataPtr[index]);
		}
		return maxValue;
	}

	int32_t max2(const std::vector<int32_t>& data)
	{
		const auto arrayLength = data.size();
		const auto dividableLength = arrayLength / 8;
		auto dataPtr = reinterpret_cast<const __m256i*>(data.data());
		auto mmMax = _mm256_set1_epi32(std::numeric_limits<int32_t>::lowest());
		for (auto index = 0; index < dividableLength; ++index) {
			const auto mmData = _mm256_lddqu_si256(dataPtr++);
			mmMax = _mm256_max_epi32(mmMax, mmData);
		}
		auto maxValues = _mm256_extracti128_si256(mmMax, 1);
		maxValues = _mm_max_epi32(maxValues, _mm256_castsi256_si128(mmMax));
		auto b = _mm_srli_si128(maxValues, 8);
		maxValues = _mm_max_epi32(maxValues, b);
		auto maxValue1 = _mm_cvtsi128_si32(maxValues);
		auto maxValue2 = _mm_extract_epi32(maxValues, 1);
		auto maxValue = std::max(maxValue1, maxValue2);

		for (auto index = dividableLength * 8; index < arrayLength; ++index) {
			maxValue = std::max(maxValue, data[index]);
		}
		return maxValue;
	}

	// add msvc attribute so this function will be inlined
	__forceinline
		int32_t min_of_8_int32(__m256i v) {
			// Step 1: Shuffle the elements to compare the first half against the second half
		__m256i permuted = _mm256_permute4x64_epi64(v, _MM_SHUFFLE(3, 1, 2, 0));
		__m256i min1 = _mm256_min_epi32(v, permuted);

		// Step 2: Shuffle and minimize again to bring the minimum to one half
		__m256i shuffled = _mm256_shuffle_epi32(min1, _MM_SHUFFLE(2, 3, 0, 1));
		__m256i min2 = _mm256_min_epi32(min1, shuffled);

		// Step 3: Perform one last shuffle and minimize to get the minimum to one lane
		__m256i min3 = _mm256_shuffle_epi32(min2, _MM_SHUFFLE(1, 0, 3, 2));
		__m256i result = _mm256_min_epi32(min2, min3);

		// Step 4: Extract the minimum (Now in every element of result) and return
		return _mm256_extract_epi32(result, 0);
	}
	//https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html#text=_mm256_shuffle_epi32&ig_expand=4931,5997
	//https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html#text=_mm256_permute4x64_epi64&ig_expand=4931

	inline const bool test = []() {

	// Test the min_of_8_int32 function
		{
			__m256i v = _mm256_setr_epi32(1, 2, 3, 4, 5, 6, 7, 8);
			int32_t min = min_of_8_int32(v);
			if (min != 1) {
				printf("min_of_8_int32 failed: %d\n", min);
				return false;
			}
		}
		return true;
	}();


	__forceinline
	int32_t max_of_8_int32(__m256i v) {
			// Step 1: Shuffle the elements to compare the first half against the second half
		__m256i permuted = _mm256_permute4x64_epi64(v, _MM_SHUFFLE(3, 1, 2, 0));
		__m256i min1 = _mm256_max_epi32(v, permuted);

		// Step 2: Shuffle and minimize again to bring the minimum to one half
		__m256i shuffled = _mm256_shuffle_epi32(min1, _MM_SHUFFLE(2, 3, 0, 1));
		__m256i min2 = _mm256_max_epi32(min1, shuffled);

		// Step 3: Perform one last shuffle and minimize to get the minimum to one lane
		__m256i min3 = _mm256_shuffle_epi32(min2, _MM_SHUFFLE(1, 0, 3, 2));
		__m256i result = _mm256_max_epi32(min2, min3);

		// Step 4: Extract the minimum (Now in every element of result) and return
		return _mm256_extract_epi32(result, 0);
	}

	__forceinline
	std::int32_t max_of_8_in32_v2(__m256i v) {
		// Finding horizontal maximum
		const auto mmMax128 = _mm_max_epi32(_mm256_extracti128_si256(v,0), _mm256_extracti128_si256(v,1));
		const auto mmMaxShuffled = _mm_shuffle_epi32(mmMax128, _MM_SHUFFLE(0,1,2,3));
		const auto mmMaxFirstTwo = _mm_max_epi32(mmMax128, mmMaxShuffled);
		const auto mmMaxFinal = _mm_max_epi32(_mm_shuffle_epi32(mmMaxFirstTwo, _MM_SHUFFLE(2,3,0,1)), mmMaxFirstTwo);
		
		return _mm_cvtsi128_si32(mmMaxFinal);
	}//https://youtu.be/AFtpRG500TU?t=1099




	int32_t max2v2(const std::vector<int32_t>& data)
	{
		const auto arrayLength = data.size();
		const auto dividableLength = arrayLength / 8;
		auto dataPtr = reinterpret_cast<const __m256i*>(data.data());
		auto mmMax = _mm256_set1_epi32(std::numeric_limits<int32_t>::lowest());
		for (auto index = 0; index < dividableLength; ++index) {
			const auto mmData = _mm256_lddqu_si256(dataPtr++);
			mmMax = _mm256_max_epi32(mmMax, mmData);
		}

		auto last8Index = arrayLength - 8;
		const auto mmData = _mm256_lddqu_si256(reinterpret_cast<const __m256i*>(&data[last8Index]));
		mmMax = _mm256_max_epi32(mmMax, mmData);
		auto maxValue = max_of_8_in32_v2(mmMax);
		return maxValue;
	}

	int32_t max3(const std::vector<int32_t>& data)
	{
		const auto arrayLength = data.size();
		const auto dividableLength = arrayLength / 8;
		auto dataPtr = reinterpret_cast<const __m256i*>(data.data());
		auto mmMax = _mm256_set1_epi32(std::numeric_limits<int32_t>::lowest());
		for (auto index = 0; index < dividableLength; ++index) {
			const auto mmData = _mm256_lddqu_si256(dataPtr++);
			mmMax = _mm256_max_epi32(mmMax, mmData);
		}
		auto last8Index = arrayLength - 8;
		const auto mmData = _mm256_lddqu_si256(reinterpret_cast<const __m256i*>(&data[last8Index]));
		mmMax = _mm256_max_epi32(mmMax, mmData);

		return max_of_8_int32(mmMax);
	}



	std::vector<int32_t> data = random_data::generate_random_data<int32_t>(32 * 1024);


	void BM_mysimd_max1(benchmark::State& state) {

		for (auto _ : state) {
			auto max1 = mysimd_max_element::max1(data);
			//auto maxValue = std::max_element(data.begin(), data.end());
			//if (max1 != *maxValue) {
			//	throw std::runtime_error("max1 != maxValue");
			//}
			benchmark::DoNotOptimize(max1);
			benchmark::ClobberMemory();
		}
		state.SetItemsProcessed(state.range(0) * state.iterations());
	}

	void BM_mysimd_max2v2(benchmark::State& state) {

		for (auto _ : state) {
			auto max1 = mysimd_max_element::max2v2(data);

			benchmark::DoNotOptimize(max1);
			benchmark::ClobberMemory();
		}
		state.SetItemsProcessed(state.range(0) * state.iterations());
	}


	void BM_mysimd_max2(benchmark::State& state) {
		for (auto _ : state) {
			auto max1 = mysimd_max_element::max2(data);
			benchmark::DoNotOptimize(max1);
			benchmark::ClobberMemory();
		}
		state.SetItemsProcessed(state.range(0) * state.iterations());
	}

	void BM_mysimd_max3(benchmark::State& state) {
		for (auto _ : state) {
			auto max1 = mysimd_max_element::max3(data);
			/*auto maxValue = std::max_element(data.begin(), data.end());
			if (max1 != *maxValue) {
				throw std::runtime_error("max1 != maxValue");
			}*/
			benchmark::DoNotOptimize(max1);
			benchmark::ClobberMemory();
		}
		state.SetItemsProcessed(state.range(0) * state.iterations());
	}

	void BM_std_max_element(benchmark::State& state) {
		for (auto _ : state) {
			auto max1 = std::max_element(data.begin(), data.end());
			benchmark::DoNotOptimize(max1);
			benchmark::ClobberMemory();
		}
		state.SetItemsProcessed(state.range(0) * state.iterations());
	}


	constexpr auto start1 = 1 << 17;
	BENCHMARK(BM_mysimd_max1)->Arg(start1);
	BENCHMARK(BM_mysimd_max2)->Arg(start1);
	BENCHMARK(BM_mysimd_max2v2)->Arg(start1);
	BENCHMARK(BM_mysimd_max3)->Arg(start1);
	BENCHMARK(BM_std_max_element)->Arg(start1);



	//-------------------------------------
	//https://youtu.be/AFtpRG500TU?t=1505
	// Rethinking if statements

	static int32_t ScalarMax1(const std::vector<int32_t>& data) {
		int32_t maxValue = std::numeric_limits<int32_t>::lowest();
		for (const auto& it : data) {
			if (maxValue < it) {
				maxValue = it;
			}
		}
		return maxValue;
	}

	static int32_t ScalarMax2(const std::vector<int32_t>& data) {
		int32_t maxValue = std::numeric_limits<int32_t>::lowest();
		for (const auto& it : data) {
			const auto isGreaterMask = -static_cast<int32_t>(maxValue >= it);
			maxValue = (maxValue & isGreaterMask) | (it & ~isGreaterMask);
		}
		return maxValue;
	}

	static int32_t ScalarMax3(const std::vector<int32_t>& data) {
		int32_t maxValue = std::numeric_limits<int32_t>::lowest();
		for (const auto& it : data) {
			maxValue = maxValue ^ ((maxValue ^ it) & -(maxValue < it));
		}
		return maxValue;
	}

	//-------------------------------------
	// Finding maximum element index with SIMD
	static int32_t FindMaxIndexSIMD(const std::vector<int32_t>& data) {
		const auto arrayLength = data.size();
		const auto dividableLength = arrayLength / 8;
		auto dataPtr = reinterpret_cast<const __m256i*>(data.data());
		auto mmMax = _mm256_set1_epi32(std::numeric_limits<int32_t>::lowest());
		auto mmPosition = _mm256_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7);

		const auto mmPositionIncrement = _mm256_set1_epi32(8);
		auto mmResultingIndexes = _mm256_setzero_si256();

		for (auto index = 0; index < dividableLength; ++index) {
			const auto mmData = _mm256_lddqu_si256(dataPtr++);

			const auto mIsGreaterMask = _mm256_cmpgt_epi32(mmData, mmMax);
			const auto mmIndexesToAdd = _mm256_and_si256(mmPosition, mIsGreaterMask);
			const auto mmIndexesToLeave = _mm256_andnot_si256(mIsGreaterMask, mmResultingIndexes);
			mmResultingIndexes = _mm256_add_epi32(mmIndexesToAdd, mmIndexesToLeave);

			mmPosition = _mm256_add_epi32(mmPosition, mmPositionIncrement);
			mmMax = _mm256_max_epi32(mmMax, mmData);

		}
		// Processing leftovers
		const auto processedLength = dividableLength * 8;
		if (processedLength != arrayLength) {
			const auto positionUpdate = 8 - (arrayLength - processedLength);
			const auto mmPositionDecrement = _mm256_set1_epi32(positionUpdate);
			mmPosition = _mm256_sub_epi32(mmPosition, mmPositionDecrement);
			const auto mmData = _mm256_lddqu_si256(reinterpret_cast<const __m256i*>(data.data() + (arrayLength - 8)));

			const auto mIsGreaterMask = _mm256_cmpgt_epi32(mmData, mmMax);
			const auto mmIndexesToAdd = _mm256_and_si256(mmPosition, mIsGreaterMask);
			const auto mmIndexesToLeave = _mm256_andnot_si256(mIsGreaterMask, mmResultingIndexes);
			mmResultingIndexes = _mm256_add_epi32(mmIndexesToAdd, mmIndexesToLeave);

			mmMax = _mm256_max_epi32(mmMax, mmData);
		}
		// todo: ...

	}
}

