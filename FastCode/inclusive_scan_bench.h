//#include <benchmark/benchmark.h>
#include <vector>
#include <algorithm>
#include <execution>
#include <benchmark/benchmark.h>

std::vector<int> v(1000);
std::vector<int> result(1000);


static void test_inclusive_scan(benchmark::State& state)
{
    // create vector of integers of size 1000000
    std::iota(v.begin(), v.end(), 1);

    // perform inclusive scan
    for (auto _ : state)
    {
        std::inclusive_scan(v.begin(), v.end(), result.begin());
    }
    benchmark::DoNotOptimize(result);
}

// Now write parallel version of inclusive_scan

static void test_inclusive_scan_par(benchmark::State& state)
{
    // create vector of integers of size 1000000
    std::iota(v.begin(), v.end(), 1);

    // perform inclusive scan
    for (auto _ : state)
    {
        std::inclusive_scan(std::execution::par, v.begin(), v.end(), result.begin());
    }
    benchmark::DoNotOptimize(result);
}

// WRITE benchmark macros
BENCHMARK(test_inclusive_scan);
BENCHMARK(test_inclusive_scan_par);
