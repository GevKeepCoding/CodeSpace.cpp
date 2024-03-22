#pragma once

/*
	Just add below line and your iostream io will get much faster.
	static int speedup = []() { ios::sync_with_stdio(false); cin.tie(nullptr); return 0; }();
*/


//#include <algorithm>
//
//
//namespace Tips_101 {
//
//	int devide_slow(int x) {
//		return x / 32;
//	}
//	// [[assume]] in: (C++ 23)
//	int devide_faster_1(int x) {
//		[[assume(x >= 0)]];
//		return x / 32;
//	}
//
//	void limiter(float* data, size_t size) {
//		[[assume(size > 0)]];
//		[[assume(size % 32 == 0)]];
//
//		for (size_t i = 0; i < size; ++i) {
//			[[assume(std::isfinite(data[i]))]];
//			data[i] = std::clamp(data[i], -1.0f, 1.0f);
//		}
//	}
//
//	// std::assume_aligned in (C++ 20)
//}
