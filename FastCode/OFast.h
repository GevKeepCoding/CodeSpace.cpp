#pragma once


#include <iostream>
#include <numeric>
#include <cmath>

namespace OFast
{
	static const int main = []() {

		constexpr double a = std::numeric_limits<double>::infinity();

		if (std::isinf(a))
			std::cout << "Inf detected.\n";
		else
			std::cout << "Inf Not detected.\n";

		if (std::isfinite(a))
			std::cout << "Is finite.\n";
		else
			std::cout << "Is Not finite.\n";
		





		return 0;
	}();
	// -ffast-math will make above code not work as expected.
	// The output will be "Inf Not detected." and "Is finite"
}