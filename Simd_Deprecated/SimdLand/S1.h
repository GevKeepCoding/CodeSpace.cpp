#pragma once
#include <immintrin.h>
#include <iostream>
#include <limits>

void shuffleBytes(__m128i& xmm0, __m128i xmm1) {
	xmm0 = _mm_shuffle_epi8(xmm0, xmm1);
}

void print(__m128i xmm) {
	unsigned char* result = reinterpret_cast<unsigned char*>(&xmm);
	for (int i = 0; i < 16; i++) {
		printf("%d ", result[i]);
	}
	printf("\n");
}
void print16(__m128i xmm)
{
	unsigned short* result = reinterpret_cast<unsigned short*>(&xmm);
	for (int i = 0; i < 8; i++) {
		printf("%d ", result[i]);
	}
	printf("\n");
}
void print32(__m128i xmm)
{
	unsigned int* result = reinterpret_cast<unsigned int*>(&xmm);
	for (int i = 0; i < 4; i++) {
		printf("%d ", result[i]);
	}
	printf("\n");
}

//Top 10 Craziest Assembly Language Instructions
//https://www.youtube.com/watch?v=Wz_xJPN7lAY
namespace simd_land1
{


int main_examples()
{
	// Example usage
	// 128 = 16 * 8;
	{

		//__m128i xmm0 = _mm_set_epi8(0, 1, 2, 3, 4, 5, 6, 7, 8,9, 10, 11, 12, 13, 14, 15);
		//print(xmm0);

		////__m128i xmm1 = _mm_set_epi8(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
		////__m128i xmm1 = _mm_set_epi8(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
		//// reverse xmm0 bytes
		////__m128i xmm1 = _mm_set_epi8(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
		//auto xmm1 = xmm0;
		//shuffleBytes(xmm0, xmm1);

		//print(xmm0);
	}

	{
		// pmaddwd xmm0, xmm1
		__m128i xmm1 = _mm_set_epi16(1, 2, 3, 4, 5, 6, 7, 8);
		__m128i xmm2 = _mm_set_epi16(1, 2, 3, 4, 5, 6, 7, 8);
		// now do the packed multiply add
		__m128i xmm3 = _mm_madd_epi16(xmm1, xmm2);
		print16(xmm3);
		print32(xmm3);
	}
	{
		constexpr auto short_max = std::numeric_limits<short>::max();
		std::cout << short_max << "---- " << short_max * short_max << std::endl;
		std::cout << int32_t(short_max) * int32_t(short_max) << std::endl;
		std::cout << std::numeric_limits<unsigned int>::max() << std::endl;
		std::cout << std::numeric_limits<int>::max() << std::endl;
		//integer can hold multiplication of largest short numbers
		//and unsigned int can hold multiplication of largest unsigned short number

	}

	{
		__m128i xmm1 = _mm_set_epi16(1, 2, 3, 4, 5, 6, 7, 8);
		// let's find the minimum
		auto min = _mm_minpos_epu16(xmm1);
		// min.m128i_u16[0] // is the min value
		// min.m128i_u16[1] // is the index of the min value
		print16(min);
	}
	return 0;
}

}