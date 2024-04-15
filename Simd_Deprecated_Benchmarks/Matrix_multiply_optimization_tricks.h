#pragma once
#include <benchmark/benchmark.h>
#include <vector>

// Adding Nested Loops Makes this Algorithm 120x FASTER ?
//https://www.youtube.com/watch?v=QGYvbsHDPxo

void multiply_scalar(const std::vector<float>& matA, const std::vector<float>& matB, std::vector<float>& matC, size_t matSize) {
	// clear matC.
	for (size_t i = 0; i < matC.size(); ++i) { matC[i] = 0; }

	// Matrix matrix multiply.
	for (size_t i = 0; i < matSize; i++) {
		for (size_t j = 0; j < matSize; j++) {
			size_t cIdx = i * matSize + j;
			for (size_t k = 0; k < matSize; k++) {
				matC[cIdx] += matA[i * matSize + k] * matB[k * matSize + j];
			}
		}
	}
} // approx 2secodns to multiply 1024x1024 matrices.


void multiply_scalar_omp(const std::vector<float>& matA, const std::vector<float>& matB, std::vector<float>& matC, size_t matSize) {
	// clear matC.
#pragma omp parallel for
	for (int i = 0; i < matC.size(); ++i) { matC[i] = 0; }

	// Matrix matrix multiply.
#pragma omp parallel for
	for (int i = 0; i < matSize; i++) {
		for (int j = 0; j < matSize; j++) {
			int cIdx = i * matSize + j;
			for (int k = 0; k < matSize; k++) {
				matC[cIdx] += matA[i * matSize + k] * matB[k * matSize + j];
			}
		}
	}
}

// to make our algorithm faster we need data locality.
void multiply_scalar_omp_data_locality(const std::vector<float>& matA, const std::vector<float>& matB, std::vector<float>& matC, size_t matSize) {
	// Transpose matrix B.
	std::vector<float> matBt(matSize * matSize);
#pragma omp parallel for
	for (int i = 0; i < matSize; i++) {
		for (int j = 0; j < matSize; j++) {
			matBt[i * matSize + j] = matB[j * matSize + i];
		}
	}
	// or mkl_somatcopy('r', 't', matSize, matSize, 1.0, matB, matSize, matSize, matBt, matSize);



	// clear matC.
#pragma omp parallel for
	for (int i = 0; i < matC.size(); ++i) { matC[i] = 0; }

	// Matrix matrix multiply.
#pragma omp parallel for
	for (int i = 0; i < matSize; i++) {
		for (int j = 0; j < matSize; j++) {
			int cIdx = i * matSize + j;
			for (int k = 0; k < matSize; k++) {
				matC[cIdx] += matA[i * matSize + k] * matB[j * matSize + k];
			}
		}
	}
} // approx 2secodns to multiply 1024x1024 matrices.


// to make our algorithm faster we need data locality.
void multiply_scalar_omp_Nested_loops(const std::vector<float>& matA, const std::vector<float>& matB,
									  std::vector<float>& matC, size_t matSize, size_t blockSize)
{
// clear matC.
#pragma omp parallel for
	for (int i = 0; i < matC.size(); ++i) { matC[i] = 0; }


	// Matrix matrix multiply. Adding nested loops
	size_t blockNum = matSize / blockSize;
#pragma omp parallel for
	for (int bi = 0; bi < blockNum; bi++) {
		for (int bj = 0; bj < blockNum; bj++) {
			for (int bk = 0; bk < blockNum; bk++) {
				// block Gemm.
				for (int i = bi * blockSize; i < (bi + 1) * blockSize; i++) {
					for (int j = bj * blockSize; j < (bj + 1) * blockSize; j++) {
						int cIdx = i * matSize + j;
						float partial = 0;
#pragma omp simd reduction(+ : partial)
						for (int k = bk * blockSize; k < (bk + 1) * blockSize; k++) {
							partial += matA[i * matSize + k] * matB[k * matSize + j];
						}
						matC[cIdx] += partial;
					}
				}
			}
		}
	}
} // approx 2secodns to multiply 1024x1024 matrices.


void multiply_scalar_omp_Nested_loops_bench(benchmark::State& state)
{
	// Generate matA, matB, matC.
	size_t matSize = 1024;
	size_t blockSize = 32;
	std::vector<float> matA(matSize * matSize);
	std::vector<float> matB(matSize * matSize);
	std::vector<float> matC(matSize * matSize);

	// multiply
	for (auto _ : state) {
		multiply_scalar_omp_Nested_loops(matA, matB, matC, matSize, blockSize);
	}
	state.SetItemsProcessed(state.iterations() * matSize * matSize);
}


void multiply_scalar_omp_data_locality_bench(benchmark::State& state) {
	// Generate matA, matB, matC.
	size_t matSize = state.range(0);
	std::vector<float> matA(matSize * matSize);
	std::vector<float> matB(matSize * matSize);
	std::vector<float> matC(matSize * matSize);

	for (auto _ : state) {
		// multiply 
		multiply_scalar_omp_data_locality(matA, matB, matC, matSize);
	}
	state.SetItemsProcessed(state.iterations() * matSize * matSize);
}



void multiply_scalar_bench(benchmark::State& state) {
	// Generate matA, matB, matC.
	size_t matSize = state.range(0);
	std::vector<float> matA(matSize * matSize);
	std::vector<float> matB(matSize * matSize);
	std::vector<float> matC(matSize * matSize);

	for (auto _ : state) {
		// multiply 
		multiply_scalar(matA, matB, matC, matSize);
	}
	state.SetItemsProcessed(state.iterations() * matSize * matSize);
}


void multiply_scalar_omp_bench(benchmark::State& state) {
	// Generate matA, matB, matC.
	size_t matSize = state.range(0);
	std::vector<float> matA(matSize * matSize);
	std::vector<float> matB(matSize * matSize);
	std::vector<float> matC(matSize * matSize);

	for (auto _ : state) {
		// multiply 
		multiply_scalar_omp(matA, matB, matC, matSize);
	}
	state.SetItemsProcessed(state.iterations() * matSize * matSize);
}


BENCHMARK(multiply_scalar_bench)->Arg(1024)->Unit(benchmark::kMillisecond);
BENCHMARK(multiply_scalar_omp_bench)->Arg(1024)->Unit(benchmark::kMillisecond);
BENCHMARK(multiply_scalar_omp_data_locality_bench)->Arg(1024)->Unit(benchmark::kMillisecond);
BENCHMARK(multiply_scalar_omp_Nested_loops_bench)->Unit(benchmark::kMillisecond);
