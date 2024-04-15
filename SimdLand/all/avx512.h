#pragma once
#include "..\Helpers\simplify.h"


namespace avx512
{
using namespace simdv1;
/*
	_mm512_cmp_epi32_mask
*/

/*
	_mm512_inserti32x4

	Inserts 4 integers from one register to another register
*/
void Example_mm512_inserti32x4()
{
	ri5 a = _mm512_setr_epi32(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
	ri1 b = _mm_setr_epi32(17, 18, 19, 20);
	ri5 c = _mm512_inserti32x4(a, b, 1);
	// c = 1, 17, 18, 19, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
	// print c
	print_zreg(c);

}


}