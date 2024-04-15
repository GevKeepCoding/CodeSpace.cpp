//BENCHMARK_STATIC_DEFINE;

//#pragma comment(lib, "shlwapi.lib")
//#include <benchmark/benchmark.h>
//BENCHMARK_MAIN();
//
#include "..\SimdLand\Helpers\simplify.h"
using namespace simdv1;
//

import std;

//
//#include <queue>
//#include <mutex>
//#include <condition_variable>
using namespace std;

void example_inserti()
{
	ri1 a = set_xvalues(1);
	ri5 b = set_zvalues(2);
	ri5 c = _mm512_inserti32x4(b, a, 3);
	print_zreg(c);
}
int main() {
	example_inserti();
}