#pragma once
#include <cstdint>
#include <bit>
#include <vector>
#include <array>

//import std;
// 
namespace SIMD_comparison_instructions {

/*

SSE/AVX Limitations:

only 2 comparison operations
	* Greater than
	* Equal

Only signed types
* int8_t, int16_t, int32_t, int64_t


*/

// repo: https://github.com/andrelrt/simd_algorithms
// new_project(with same ideas but more organized) litesimd: https://github.com/andrelrt/litesimd
//"SIMD comparison instructions" by André Tupinambá
//https://www.youtube.com/watch?v=e-0kMyOIdP0
	uint32_t greater_than_index(int32_t key, __m128i cmp) {
		__m128i simdKey = _mm_set1_epi32(key);
		__m128i mask = _mm_cmpgt_epi32(simdKey, cmp);
		uint32_t bitmask = _mm_movemask_epi8(mask);

		unsigned long index = 0;
		_BitScanReverse(&index, bitmask);

		return (bitmask == 0) ? 0
			: (index + 1) / 4;
	}
	// _bit_scan_reverse(b00000000000000000000000000001111) == 3
	// unsigned long index = 0;
	// _BitScanReverse(&index, x);
	// std::println("{}", index);


	class nway {
	public:
		using const_iterator = std::vector<int32_t>::const_iterator;
		nway(const std::vector<int32_t>& v) : vec_(v) {}

		void build_index();
		const_iterator find(int32_t key);

	private:
		__m128i cmp_;
		const std::vector<int32_t>& vec_;
		std::array<const_iterator, 4 + 2> ranges_;
	};

	void nway::build_index() {
		size_t step = vec_.size() / 5; // 5 = 1 + SIMD size
		int32_t* pCmp = reinterpret_cast<int32_t*>(&cmp_);
		auto it = vec_.begin();
		ranges_[0] = it;
		for (size_t i = 0; i < 4; ++i) { // 4 is simd size
			std::advance(it, step);
			ranges_[i + 1] = it;
			pCmp[i] = *it;
		}
		ranges_[5] = std::prev(vec_.end());
	}

	nway::const_iterator nway::find(int32_t key) {
		uint32_t idx = greater_than_index(key, cmp_);
		auto end = std::next(ranges_[idx + 1]);
		auto it = std::lower_bound(ranges_[idx],
								   end, key);
		return (it != end && !(key < *it))
			? it : vec_.end();
	}

	// N-Way Tree:
	// https://youtu.be/e-0kMyOIdP0?t=1336

	// SSE
	void vector_to_lower(std::string& str)
	{
		using reg = __m128i;
		reg* data = (reg*)str.data();
		size_t size = str.size() & ~(16 - 1);//16=simd size
		for (size_t i = 0; i < size; i += 16)
		{
			reg low = _mm_cmpgt_epi8(*data, _mm_set1_epi8('A' - 1));
			reg high = _mm_cmpgt_epi8(
				_mm_set1_epi8('Z' + 1), *data);

			reg mask = _mm_and_si128(low, high);

			reg lower = _mm_add_epi8(*data, _mm_set1_epi8(0x20));
			*data = _mm_blendv_epi8(*data, lower, mask);
			++data;
		}

		for (size_t i = size; i < str.size(); ++i)
			if ('A' <= str[i] && str[i] <= 'Z')
				str[i] += 0x20;
	} // 8x faster than scalar code | avx is 12x faster

}





void main_1() {
	int x = 0b00000000000000000000000000001111;
	std::println("{}", _mm_popcnt_u32(x));
	unsigned long index = 0;
	_BitScanReverse(&index, x);
	std::println("{}", index);
}