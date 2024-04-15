#pragma comment(lib, "shlwapi.lib")
#include <benchmark/benchmark.h>

#include "mmap_save_load.h"


// Define a benchmark function
static void BM_MemoryMappedFile(benchmark::State& state) {

  for (auto _ : state) {
    // Access the mapped file
  }
  //destroy_mapped_file(file_name);
}

//BENCHMARK(BM_MemoryMappedFile);
//BENCHMARK_MAIN();


int main() {

  read_write_mmap();



  return 0;
}