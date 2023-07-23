#pragma once
#include <climits>

import std;

int devide_int_int(int a, int b)
{
	return a / b;
}

int modulo_devide_int_int(int a, int b)
{
	return a % b;
}

void IntegerOverflowExamples()
{
	std::printf("%d\n", INT_MIN);
	std::printf("%d\n", devide_int_int(INT_MIN, -1));
	std::printf("%d\n", modulo_devide_int_int(INT_MIN, -1));



	// more C++ quizzes
	short a = 1;
	std::cout << (sizeof(+a))["23456"] << std::endl;
	std::cout << sizeof(+a) ["23456"] << std::endl;
	std::cout << sizeof((+a)["23456"] ) << std::endl;
}