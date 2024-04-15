#pragma once
#include <immintrin.h>
#include <stdio.h>
#include "../Helper/simplify.h"


namespace m128 {

}
namespace m256 {

}
namespace m512
{


	int equal_zvalues(ri5 a, ri5 b)
	{
		short r = _mm512_cmp_epi32_mask(a, b, _MM_CMPINT_NE);
		return r == 0;
	}

	// let's write example of usage of: _mm512_inserti32x4
	// this function is used to insert 4 integers from one register to another register
	void example_inserti()
	{
		ri1 a = set_xvalues(1);
		ri5 b = set_zvalues(2);
		ri5 c = _mm512_inserti32x4(b, a, 0);
		print_zreg(c);
	}



}

int main()
{

	ri1 a = set_xvalues(1);
	ri5 b = set_zvalues(2);
	ri5 c = _mm512_inserti32x4(b, a, 0);
	print_zreg(c);
}