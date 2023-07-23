#pragma once

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cfenv>
#include <bitset>

void PrintCurrentRoundingMode()
{
	unsigned int cw;
	_controlfp_s(&cw, 0, 0);
	cw &= _MCW_RC;
	printf("Rounding mode is ");
	switch (cw) {
	case _RC_NEAR:
		printf("round to nearest\n");
		break;
	case _RC_UP:
		printf("round up\n");
		break;
	case _RC_DOWN:
		printf("round down\n");
		break;
	case _RC_CHOP:
		printf("chop\n");
		break;
	}
}
static int file_static = [] { std::puts("init file static "); return 1; }();

void MoreRoundingModeExamples()
{
	// #include <cfenv>
	// FE_DOWNWARD: toward negative infinity (2.3 -> 2.0, -2.3 -> -3)
	// FE_UPWARD: toward positive infinity (2.3 -> 3.0, -2.3 -> -2.0)
	// FE_TOWARDZERO: toward zero (2.3 -> 2.0, -2.3 -> 2.0)
	// FE_TONEAREST: to nearest (2.5 -> 2.0, 3.5 -> 4.0)
	// std::fesetround(FE_DOWNWARD); to set rounding mode

	float f = 1.5f;

	std::printf("f = %f\n", f);
	std::printf("f = %f\n", std::floor(f)); // toward negative infinity
	std::printf("f = %f\n", std::ceil(f)); // toward positive infinity
	std::printf("f = %f\n", std::trunc(f)); // toward zero
	std::printf("f = %f\n", std::round(f)); // to nearest integer

	std::printf("f = %f\n", std::nearbyint(f)); // to nearest integer using current rounding mode

	PrintCurrentRoundingMode();

	// example of changing rounding mode
	// Round is rounding to up when .5 is seen, nearbyint is rounding to nearest even integer

	std::printf("f = %f\n", std::round(f)); // to nearest integer using current rounding mode

	std::fesetround(FE_DOWNWARD);
	std::printf("f = %f\n", std::round(f)); // to nearest integer using current rounding mode
	std::printf("f = %f\n", std::nearbyint(f)); // to nearest integer using current rounding mode

	
	/*
		Floating point exceptions
		
		FE_DIVBYZERO: division by zero
		FE_INEXACT: inexact result
		FE_INVALID: domain error(eg.sqrt(-1))
		FE_OVERFLOW: overflow
		FE_UNDERFLOW: underflow (too close to zero)

		std::feraiseexcept raise floating point exception manually
		std::fetestexcept test for floating point exception
		std::feclearexcept clear floating point exception
	*/
	 float x = 0.0;
	 std::feclearexcept(FE_ALL_EXCEPT);
	 std::printf("%f\n", 1.0/x); // division by zero
	 std::printf("%s\n", std::fetestexcept(FE_DIVBYZERO) ? "division by zero" : "okey");
	 
	 std::feclearexcept(FE_ALL_EXCEPT);
	 std::printf("%f\n", 0.0
				 / x); // division by zero
	 std::printf("%s\n", std::fetestexcept(FE_DIVBYZERO) ? "division by zero" : "okey");
	 // print binary representation of x
	 float y1 = 0 / x;
	 float y2 = 1 / x;
	 std::cout << std::bitset<sizeof(x) * 8>(*(unsigned long long*)&y1) << std::endl;
	 std::cout << std::bitset<sizeof(x) * 8>(*(unsigned long long*)&y2) << std::endl;

	 // we can store floating point number | pointer type in a single floating variable if it can't be nan.
	 // it's called nan-boxing

	 /*
		namespace std
		{
			double nan(const char* tagp);
		}

		Nan boxing: http://piotrduperas.com/posts/nan-boxing

	 */
	 static int function_static = [] { std::puts("init function static"); return 1; }();
	 ++function_static;
	 ++file_static;
	 std::cout << "function_static " << function_static << std::endl;
	 std::cout << "file_static " << file_static << std::endl;
}



//int main() {
//
//	MoreRoundingModeExamples();
//
//	float f = 1.2f;
//	int* asInt = reinterpret_cast<int*>(&f);
//	printf("%1.30f\n", f);
//
//	std::cout << "Float " << std::fixed << std::setprecision(30) << f << std::endl;
//	std::cout << "AsInt " << *asInt << std::endl;
//
//	++(*asInt);
//	std::cout << "After increment" << std::fixed << std::setprecision(30) << f << std::endl;
//}