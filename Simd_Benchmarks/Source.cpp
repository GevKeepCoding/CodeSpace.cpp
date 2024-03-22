//#pragma comment(lib, "benchmark.lib")
//#pragma comment(lib, "benchmark_main.lib")
#pragma comment(lib, "shlwapi.lib")
#include <benchmark/benchmark.h>
//#include "Slow_Simd.h"
//#include "Vectorized_transform_reduce_perf.h"

//import std;
using std::vector;



bool test = []() {

	vector<int32_t> data{1,2,3,4,5,6,7,8,9};
	auto maxValue = FindMaxValue(data);


	return true;
}();



BENCHMARK_MAIN();