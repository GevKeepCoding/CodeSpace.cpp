#pragma once
#include <climits>
#include <bit>

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
	std::cout << sizeof(+a)["23456"] << std::endl;
	std::cout << sizeof((+a)["23456"]) << std::endl;
}


//Bit Hacks from Beginner to Advanced - 11 Amazing Bit Twiddling Techniques : https://www.youtube.com/watch?v=ZRNO-ewsNcQ
namespace simple_bits
{

int setBit(int n, int k)
{
	return n | (1 << k);
}

int resetBit(int n, int k)
{
	return ~(1 << k) & n;
}

int toggleBit(int n, int k)
{
	return (1 << k) ^ n;
}

bool isBitSet(int n, int k)
{
	return (n & (1 << k)) != 0;
}

int convertTrailingZeroesToOnes(int n) {
	return (n - 1) | n;
}

// Two's complement means:		-x = ~x + 1 ==> x + ~x = -1
// x and -x will share only the least significant 1 bit

int getRightmostSetBit(int n)
{
	return n & -n;
}

// copies bits from b into a, if corresponding bits in mask are set to 1
void maskedCopy(int& a, int b, int mask)
{
	a = (b & mask) | (a & ~mask);
}

// swap two bits at positions a and b
void swapBits(int& x, int a, int b)
{
	auto P = (x >> a) ^ (x >> b) & 1;
	x ^= (P << a);
	x ^= (P << b);
}

int numberOfSetBits(int n)
{
	return std::popcount(unsigned(n));
}

// counting bit islands(consecutive 1's are considers as Islands)
int countIslands(uint32_t n)
{
	return (n & 1) + std::popcount((n ^ (n >> 1))) / 2;
}

// BSF (bit scan forwards)
// x86 cpu's have special instruction for this.
// counts trailing zeroes
int bsf(uint32_t n)
{
	return std::countr_zero(n);
}
int bsf_handwritten_slow(int32_t x) {
	if (x == 0) return -1;
	x = x & -x;
	int count = 0;

	if ((x & 0xffff0000) != 0) count += 16;
	if ((x & 0xff00ff00) != 0) count += 8;
	if ((x & 0xf0f0f0f0) != 0) count += 4;
	if ((x & 0xcccccccc) != 0) count += 2;
	if ((x & 0xaaaaaaaa) != 0) count += 1;
	return count;
}


// Next Lexicographically greater number with same number of 1 bits
uint32_t nextLexicographicPermutation(int32_t x)
{
	auto t = x | (x - 1);
	return (t + 1) | (((~t & -(~t) - 1) >> (std::countr_zero(uint32_t(x)) + 1)));
}

//-----------------------------------------------------------
auto MinValue(int x, int y) {
	return y ^ ((x ^ y) & -(x < y));
}
// this trick avoids branches but it doesn't give us guaranted performance.
// (x+y)modn, assuming 0 <= x < n and 0 <= y <n.
auto ModuloPlus(int x, int y, int n)
{
	auto z = x + y;
	auto r = z - (n & -(z >= n));
	return r;
}

// compute 2to the power of lgn
uint64_t pow2(uint64_t n)
{
	if (n == 0) return 1;
	--n; // makes ceil(logn) -1 bit set.
	n |= n >> 1;
	n |= n >> 2;
	n |= n >> 4;
	n |= n >> 8;
	n |= n >> 16;
	n |= n >> 32;
	return n + 1;
}

// Compute lgx, where x is a power of 2.
auto compute_lgx(uint64_t x)
{
	/*
	deBruijn sequence s of length 2^k is a cyclic sequence such that each
	of the 2^k strings of length k occurs exactly once as a substring of s.

	Example: k = 3
	00011101
	8 subsequence of length 3: 000, 001, 011, 111, 110, 101, 010, 100
	
	const int convert[8] = {0,1,6,2,7,5,4,3};

	0b00011101 * 16 = 0b11010000
	0b11010000 >> 5 = 0b00000110 = 6
	convert[6] = 4 ==> lg(16) = 4

	*/
	const uint64_t deBruijn = 0x022fdd63cc95386d;
	const int convert[64] = {
		0, 1, 2, 53, 3, 7, 54, 27,
		4, 38, 41, 8, 34, 55, 48, 28,
		62, 5, 39, 46, 44, 42, 22, 9,
		24, 35, 59, 56, 49, 18, 29, 11,
		63, 52, 6, 26, 37, 40, 33, 47,
		61, 45, 43, 21, 23, 58, 17, 10,
		51, 25, 36, 32, 60, 20, 57, 16,
		50, 31, 19, 15, 30, 14, 13, 12
	};

	auto r = convert[(x * deBruijn) >> 58];
	return r;
}


}
using namespace simple_bits;
using namespace std;

int main() {
	cout << compute_lgx(2);
}