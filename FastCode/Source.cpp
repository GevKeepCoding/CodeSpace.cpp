//#pragma comment(lib, "benchmark.lib")
//#pragma comment(lib, "benchmark_main.lib")
#pragma comment(lib, "shlwapi.lib")
#include <benchmark/benchmark.h>


#include "Simd1.h"
#include "substr_search_learning.h"
#include "Stolin01.h"
#include "01Stolin.h"

#include "BranchLessProgramming.h"
#include "01BranchPredictions.h"
#include "OFast.h"


BENCHMARK_MAIN();