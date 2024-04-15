#pragma once


#include "../Helpers/simplify.h"


namespace fast_addition_of_8_integers
{
using namespace simdv1;

using reg = __m256i;

int sum_8_integers(reg x)
{
	reg t = _mm256_hadd_epi32(x, x);
	t = _mm256_hadd_epi32(t, t);
	return _mm256_extract_epi32(t, 0) + _mm256_extract_epi32(t, 4);
}

inline namespace other_iml
{
int hsum1(reg x)
{
	int t[8], s = 0;
	_mm256_storeu_si256((reg*)t, x);
	for (int i = 0; i < 8; ++i) {
		s += t[i];
	}
	return s;
}


int hsum2(reg x)
{
	int s = 0;
	for (int i = 0; i < 8; ++i) {
		s += x.m256i_i32[i];
	}
	return s;
}

int hsum_copy(reg x)
{
	alignas(64) int t[8];
	int s = 0;
	_mm256_store_si256((reg*)t, x);
	for (int i = 0; i < 8; ++i) {
		s += t[i];
	}
	return s;
}


}
}

//SIMD и ручная векторизация (доп. семинар для первого курса по языку C и алгоритмам)
// https://github.com/tilir/c-graduate/blob/master/simd/simd.h
namespace find_simd
{
using namespace simdv1;
namespace v1
{
int find(const int* a, int n, int x)
{
	int i;
	for (i = 0; i < n; i++)
		if (a[i] == x)
			return i;
	return -1;
}


int find_index_of_first_equal_zvalues(ri5 a, ri5 b)
{
	unsigned short mask = _mm512_cmp_epi32_mask(a, b, _MM_CMPINT_EQ);
	if (mask != 0)
	{
		// i need to return first non zero position in mask
		return std::countr_zero(mask);
	}
	return -1;
}

int find_index_of_first_equal_yvalues(ri2 a, ri2 b)
{
	unsigned short mask = _mm256_cmp_epi32_mask(a, b, _MM_CMPINT_EQ);
	if (mask != 0)
	{
		// i need to return first non zero position in mask
		return std::countr_zero(mask);
	}
	return -1;
}


int find_simd(const int* a, int n, int x)
{
	int i;
	ri5 needle = set_zvalues(x);
	int mainsz = (n / 16) * 16;

	for (i = 0; i < mainsz; i += 16)
	{
		ri5 undertest = load_zvalue_unaligned(a + i);
		int m = find_index_of_first_equal_zvalues(needle, undertest);
		if (m != -1)
			return i + m;
	}

	for (i = mainsz; i < n; i++)
		if (a[i] == x)
			return i;
	return -1;
}



//BENCHMARK(find_simd_bench); // fastest one: todo add loop unrolling (look to Stolin)
//BENCHMARK(find_bench);
//BENCHMARK(find_stl_bench);
//BENCHMARK_MAIN();
}





}


//https://www.youtube.com/watch?v=AFtpRG500TU&list=TLPQMDEwMTIwMjXc4frbjEmdNg&index=8
namespace find_max_simd
{
	//3 microseconds
	int32_t find_max1(std::vector<int32_t>& data)
	{
		auto dividableLength = data.size() / 8;
		auto arrayLength = data.size();
		auto dataPtr = data.data();
		auto index = 0;
		auto mmMax = _mm256_set1_epi32(std::numeric_limits<int32_t>::lowest());
		for (; index + 7 < arrayLength; index += 8) {
			auto mmData = _mm256_lddqu_si256(reinterpret_cast<const __m256i*>(&dataPtr[index]));
			mmMax = _mm256_max_epi32(mmData, mmMax);
		}
		// further logic for remaining elements.
		auto maxValue = std::numeric_limits<int32_t>::lowest();
		const auto processedLength = dividableLength * 8;
		for (auto index = processedLength; index < arrayLength; ++index) {
			if (maxValue > dataPtr[index]) {
				maxValue = dataPtr[index];
			}
		}
		return maxValue;
	}
	// 1.5 microseconds
	int32_t find_max2(std::vector<int32_t>& data) {
		// if size < 8 do simple find.
		// else below code.
		auto dividableLength = data.size() / 8;
		auto arrayLength = data.size();
		auto dataPtr = reinterpret_cast<const __m256i*>(data.data());
		auto mmMax = _mm256_set1_epi32(std::numeric_limits<int32_t>::lowest());

		for (auto index = 0; index < dividableLength; index++) {
			const auto mmData = _mm256_lddqu_si256(dataPtr++);
			mmMax = _mm256_max_epi32(mmData, mmMax);
		}
		// further logic
		const auto processedLength = dividableLength * 8;
		if (processedLength != arrayLength) {
			auto mmData = _mm256_lddqu_si256(reinterpret_cast<const __m256i*>(data.data() + (arrayLength - 8)));
			mmMax = _mm256_max_epi32(mmData, mmMax);
		}
		return 0;
	}
	// we have 2 ways to shuffle elements.
	// _mm256_permutexxx(3 latency) 32byte (8 integers)
	// or _mm_shufflexx(1 latency) 16 byte (4 integers)
	/*
				Algorithm to find max of 8 integers.
	12345678
	1234 5678
	5678
	5678 7865
	7878
	7878 8787
	8888
	8
	*/
	// Finding horizontal max
	int32_t find_max_8elements(__m256i mmMax)
	{
		auto mmMax128 = _mm_max_epi32(_mm256_extracti128_si256(mmMax, 0),
			_mm256_extracti128_si256(mmMax, 1));
		auto mmMaxShuffled = _mm_shuffle_epi32(mmMax128, _MM_SHUFFLE(0, 1, 2, 3));
		auto mmMaxFirstTwo = _mm_max_epi32(mmMax128, mmMaxShuffled);
		auto mmMaxFinal = _mm_max_epi32(_mm_shuffle_epi32(mmMaxFirstTwo, _MM_SHUFFLE(2, 3, 0, 1)), mmMaxFirstTwo);
		return _mm_cvtsi128_si32(mmMaxFinal);
	}


	int32_t findMaxIndexSimd(std::vector<int32_t> data) {
		auto arrayLength = data.size();
		auto dividableLength = arrayLength / 8;
		auto dataPtr = reinterpret_cast<const __m256i*>(data.data());
		auto mmMax = _mm256_set1_epi32(std::numeric_limits<int32_t>::lowest());
		auto mmPosition = _mm256_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7);
		const auto mmPositionIncrement = _mm256_set1_epi32(8);
		auto mmResultingIndexes = _mm256_setzero_si256();
		for (auto index = 0; index < dividableLength; index++) {
			auto mmData = _mm256_lddqu_si256(dataPtr++);
			auto mmIsGreaterMask = _mm256_cmpgt_epi32(mmData, mmMax);
			auto mmIndexesToAdd = _mm256_and_si256(mmIsGreaterMask, mmResultingIndexes);
			mmResultingIndexes = _mm256_add_epi32(mmPosition, mmPositionIncrement);
			mmMax = _mm256_max_epi32(mmData, mmMax);
		}
		// processing leftovers
		auto processedLength = dividableLength * 8;
		if (processedLength != arrayLength) {
			auto positionUpdate = static_cast<int>(8 - (arrayLength - processedLength));
			auto mmPositionDecrement = _mm256_set1_epi32(positionUpdate);
			mmPosition = _mm256_sub_epi32(mmPosition, mmPositionDecrement);
			auto mmData = _mm256_lddqu_si256(reinterpret_cast<const __m256i*>(data.data() + (arrayLength - 8)));
			auto mmIsGreaterMask = _mm256_cmpgt_epi32(mmData, mmMax);
			auto mmIndexesToAdd = _mm256_and_si256(mmPosition, mmIsGreaterMask);
			auto mmIndexesToLeave = _mm256_andnot_si256(mmIsGreaterMask, mmResultingIndexes);
			mmResultingIndexes = _mm256_add_epi32(mmIndexesToAdd, mmIndexesToLeave);

			mmMax = _mm256_max_epi32(mmData, mmMax);
		}

		// Compute and return the index of maximum element
		return find_max_8elements(mmResultingIndexes);
	}

};



namespace broadcast_simd_gather
{
	void access_values_using_simd() {

		std::vector<int> indeces = { 1, 2, 3,4,5,6,7,8,9 };
		std::vector<int> values = { 1, 2, 3,4,5,6,7,8,9,10,11 };

		// write avx2 broadcast example
		__m256i vindex = _mm256_loadu_si256((__m256i*)indeces.data());
		__m256i vvalue = _mm256_i32gather_epi32(values.data(), vindex, 4); // 4 is the size of int

		int* p = (int*)&vvalue;
		for (int i = 0; i < 8; i++) {
			std::cout << p[i] << std::endl;
		}
	}
}