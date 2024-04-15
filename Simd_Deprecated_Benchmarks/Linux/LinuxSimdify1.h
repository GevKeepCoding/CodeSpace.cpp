#pragma once
#include <immintrin.h>
#include <iostream>


namespace example_1 {
	// avx512 is prefers data to be aligned to 64 bytes.
	const int count = 32;
	float a[count] __attribute__((aligned(64)));
	float b[count] __attribute__((aligned(64)));
	float c[count] __attribute__((aligned(64)));


	void actual_algorithm(int* a, int* b, int* c) {
		// giving hint that data are aligned
		__assume_aligned(a, 64); __assume_aligned(b, 64); __assume_aligned(c, 64);
		for (int i = 0; i < count; ++i) {
			c[i] = a[i] + b[i];
		}
	}
	// one more options is to add pragma when you are sure that the loop does not have dependencies
	// #pragma ivdep
	// for(int i =0; i < count;i++) ...

	// ICC compiler: icc -O2 -xHost -qopt-report=5
	// to override comiler to use avx512 instead fo avx2. add -qopt-zmm-usage=high
	// good learning resource:
	// Turtorial: www.intel.com/content/www/us/en/develop/documentation/get-started-with-advisor
	// -xAVX
	// -xCORE-AVX2
	// -xCORE-AVX512
	// intel vectorization introduction: https://www.intel.com/content/dam/develop/external/us/en/documents/vectorization-performance-quantifi-755040.pdf


}

#define REPEAT2(x) {x} {x}
#define REPEAT4(x) REPEAT2(REPEAT2(x))
#define REPEAT8(x) REPEAT4(REPEAT2(x))
#define REPEAT16(x) REPEAT8(REPEAT2(x))
#define REPEAT32(x) REPEAT16(REPEAT2(x))
#define REPEAT64(x) REPEAT32(REPEAT2(x))
#define REPEAT(x) REPEAT64(REPEAT2(x))