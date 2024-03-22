#pragma once
#include <stdlib.h>
#include <random>
#include <benchmark\benchmark.h>


//Parsing JSON Really Quickly : Lessons Learned		https://www.youtube.com/watch?v=wlvKAT7SZIQ
// https://github.com/lemire/simdjson
// https://arxiv.org/abs/1902.08318

// Trick #1. avoid hard-to-predict branches.
namespace trick1 {
	std::random_device rd{};
	std::mt19937 mt(rd());
	std::uniform_int_distribution dist(0, 100);

	auto fill = [](std::vector<int>& out, int index, int size) {
		while (size != 0)
		{
			out[index] = dist(mt);
			index += 1;
			--size;
		}
	}; // 3 cycles

	auto fill_only_odds = [](std::vector<int>& out, int index, int size) {
		while (size != 0)
		{
			auto val = dist(mt);
			if (val % 2 == 1) {
				out[index] = val;
				index += 1;
			}
			--size;
		}
	}; // 15 cycles

	auto fill_only_odds_branchless = [](std::vector<int>& out, int index, int size) {
		while (size != 0)
		{
			auto val = dist(mt);
			out[index] = val;
			index += val & 1;
			--size;
		}
	}; // back to 4 cycles


	template<class Fun>
	void bm_fill(benchmark::State& state) {
		std::vector v(state.range(0), 0);
		Fun fn;
		for (auto _ : state) {
			fn(v, 0, v.size());
			benchmark::DoNotOptimize(v);
		}
	}

	const auto size = 4u * 1024u;

	BENCHMARK(bm_fill<decltype(fill)>)->Range(size, size * 2);
	BENCHMARK(bm_fill<decltype(fill_only_odds)>)->Range(size, size * 2);
	BENCHMARK(bm_fill<decltype(fill_only_odds_branchless)>)->Range(size, size * 2);
}
/*
Saturated subtraction: x - 244 is non-zero if and only if x > 244.
_mm256_subs_epi8(current_bytes, 244);
32 bytes at once.
*/

bool test = []() {
	__m256i current_bytes = _mm256_setr_epi32(1, 2, 3, 4, 5, 6, 244, 245);
	__m256i b = _mm256_set1_epi32(244);
	auto result = _mm256_subs_epu8(current_bytes, b);
	// test if result contains non zero element
	auto mask = _mm256_movemask_epi8(result);
	if (mask) {
		//...
	}


	return true;
}();
/*
Classifying characters:
comma (0x2c) ,
colon (0x3a) :
brackets (0x5b, 0x5d, 0x7b, 0x7d) [ ] { }
white-space (0x09, 0x0a, 0x0d, 0x20) tab, newline, carriage return, space
others

Device a byte into two 'nibbles'
0x2c is 2 (high nibble) and c (low nibble)
there are 16 possible low nibbles and 16 possible high nibbles.

ARM NEON and x64 processors have instruction to lookup 16-byte tables in a vectorized manner(16 values at a time): pshufb, tbl


// start with an array of 4-bit values
[1,1,0,2, 0, 5, 10, 15, 7,8,13,9,0,13,5,1]
Create a lookup table
[200, ....215]
0 => 200 , 1 => 201, 2 => 202. ...
Result:
[201,201, 200, 2002, ...201]


//-------

comma (0x2c) ,														| 1	
colon (0x3a) :														| 2
brackets (0x5b, 0x5d, 0x7b, 0x7d) [ ] { }							| 4 
white-space (0x09, 0x0a, 0x0d) tab, newline, carriage return	| 8
white-space (0x20):													| 16
others:																| 0

H1 and H2 tables such as the bitwise and of the look classify the characters as above.
H1(low(c)) & H2(high(c)) => H1 & H2

*/
//#include <arm_neon.h>

namespace trick2 {
	// ARM NEON
	//void classify_characters_Example() {
	//	const uint8x16_t low_nibble_mask = (uint8x16_t){16, 0,0,0,0,0,0,0,0,8,12,1,2,9,0,0};
	//	const uint8x16_t high_nibble_mask = (uint8x16_t){8,0,18,4,1,0,1,0,0,0, 3,2,1,0,0};
	//	const uint8x16_t low_nib_and_mask = vmovq_n_u8(0xf);

	//	//Five instructions:
	//	uint8x16_t nib_lo = vandq_u8(chunk, low_nibble_mask);
	//	uint8x16_t nib_hi = vandq_u8(chunk, 4);
	//	uint8x16_t shuf_lo = vqtbl1q_u8(low_nibble_mask, nib_lo);
	//	uint8x16_t shuf_hi = vqtbl1q_u8(high_nibble_mask, nib_hi);
	//	return vandq_u8(shuf_lo, shuf_hi);

	//}


	// x64
	// sow me example of curious multiplication simd instruction usage which is known in cryptography.
	void example(auto quote) {
		// _1___1___1__1 (quotes)
		// will become
		// _11111___1111 (string region).
		auto mask = quote xor (quote << 1);
		mask = mask xor (mask << 2);
		mask = mask xor (mask << 4);
		mask = mask xor (mask << 8);
		mask = mask xor (mask << 16);
		// mask now contains 1s in the string region.
	}
}