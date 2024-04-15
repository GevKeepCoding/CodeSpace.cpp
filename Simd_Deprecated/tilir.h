#pragma once
#include <immintrin.h>
#include <cstdint>
#include <stdio.h>
#include <iostream>
#include <bitset>
#include <cassert>

// https://github.com/tilir/c-graduate/blob/master/simd/simd.h


namespace tilir_simd
{
using ri512 = __m512i;

ri512 set_zvalues(int x)
{
	return _mm512_set1_epi32(x);
}

void print_zreg(ri512 r)
{
	int32_t* p = (int32_t*)&r;
	for (int i = 0; i < 16; i++)
	{
		printf("%d ", p[i]);
	}
	printf("\n");
	{
		// int vals[16] __attribute__((align(64)); // gcc
		// __declspec(align(64)) int vals[16]; // msvc
		alignas(64) int vals[16]; // best way
		_mm512_store_epi32(vals, r);
		for (int i = 0; i < 16; i++)
		{
			printf("%d ", vals[i]);
		}
	}
}

int equal_zvalues(ri512 a, ri512 b)
{
	short r = _mm512_cmp_epi32_mask(a, b, _MM_CMPINT_NE);
	return r == 0;
}

int find_index_of_first_equal_zvalues(ri512 a, ri512 b)
{
	unsigned short mask = _mm512_cmp_epi32_mask(a, b, _MM_CMPINT_EQ);
	if (mask != 0)
	{
		// i need to return first non zero position in mask
		return std::countr_zero(mask);
	}
	return -1;
}
// now let's optimize find algorithm using above simd functions.
int find(const int* a, int n, int x)
{
	int i;
	for (i = 0; i < n; i++)
		if (a[i] == x)
			return i;
	return -1;
}
ri512 load_zvalue_unaligned(const int* a)
{
	return _mm512_loadu_si512(a);
}

int find_simd(const int* a, int n, int x)
{
	int i;
	ri512 needle = set_zvalues(x);
	int mainsz = (n/16)*16;

	for (i = 0; i < mainsz; i += 16)
	{
		ri512 undertest = load_zvalue_unaligned(a + i);
		int m = find_index_of_first_equal_zvalues(needle, undertest);
		if (m != -1)
			return i + m;
	}

	for (i = mainsz; i < n; i++)
		if (a[i] == x)
			return i;
	return -1;
}


}
using namespace tilir_simd;
int main_tilir() {


	{
		__m128i xmm1 = _mm_set_epi16(1, 2, 3, 4, 5, 6, 7, 8);
		__m128i xmm2 = _mm_set_epi16(1, 2, 3, 4, 5, 6, 7, 8);
		// now do the packed multiply add
		__m128i xmm3 = _mm_madd_epi16(xmm1, xmm2);
	}


	ri512 r = _mm512_setr_epi32(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
	print_zreg(r);

	{
		ri512 r2 = _mm512_setr_epi32(1, 3, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
		auto res = equal_zvalues(r, r2);
		printf("equal_zvalues: %d\n", res);
		std::bitset<32> binary(res);
		std::cout << binary << std::endl;
	}

	{
		unsigned short x = 0b0000000000000010;
		auto res = std::countr_zero(x);
		printf("countl_zero: %d\n", res);

	}

	{
		// testing find_simd == find
		int a[1000];
		for (int i = 0; i < 1000; i++)
		{
			a[i] = i;
		}

		for (int i = 0; i < 1000; i++) {
			int res = find(a, 1000, i);
			int res2 = find_simd(a, 1000,i);
			assert(res == res2);
		}

		std::cout << "Same";
	}


	return 0;
}







