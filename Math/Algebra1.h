#pragma once

namespace algebra_1
{
int power(int a, int b) {
	if (b == 0) return 1;
	int tmp = power(a, b / 2);
	int result = tmp * tmp;
	if (b % 2 == 1) result *= a;
	return result;
}

int power_iterative(int a, int b) {
	if (b == 0) return 1;
	int result = 1;
	while (b > 0) {
		if (b % 2) result *= a;
		a *= a;
		b /= 2;
	}
	return result;
}// O(log b)

// p is prime:
// a^(p-1) == 1
// a^(p-2) = 1/a

int inverse(int a, int p) {
	return power(a, p - 2);
}



}