#pragma once


#include "../Helpers/simplify.h"


namespace fast_addition_of_8_integers
{
using namespace simdv1;

using reg = __m256i;

int hsum(reg x)
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


