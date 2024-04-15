#pragma once
#include <immintrin.h>
#include <iostream>


using ri1 = __m128i;
using ri2 = __m256i;
using ri5 = __m512i;


ri5 set_zvalues(int x)
{
	return _mm512_set1_epi32(x);
}

ri2 set_yvalues(int x)
{
	return _mm256_set1_epi32(x);
}

ri1 set_xvalues(int x)
{
	return _mm_set1_epi32(x);
}


ri5 load_zvalue_unaligned(const int* a)
{
	return _mm512_loadu_si512(a);
}
ri2 load_yvalues_unaligned(const int* a)
{
	return _mm256_loadu_si256((ri2*)a);
}

ri1 load_xvalues_unaligned(const int* a)
{
	return _mm_loadu_si128((ri1*)a);
}

void print_zreg(__m512i r)
{
	alignas(64) int vals[16];
	_mm512_store_epi32(vals, r);
	for (int i = 0; i < 16; i++)
	{
		std::println(std::cout, "{} ", vals[i]);
	}
}

void print_yreg(__m256i r) {
	alignas(64) int vals[8];
	_mm256_storeu_epi32(vals, r);
	for (int i = 0; i < 8; i++)
	{
		std::println(std::cout, "{} ", vals[i]);
	}
}