#include <benchmark/benchmark.h>
#include <iostream>
#include <vector>

namespace fast_matrix_multiplication_tricks
{

const int n = 1000;

// Benchmark function
void BM_MatrixMultiplication(benchmark::State& state, auto multiply) {
	std::vector<std::vector<int>> A(n, std::vector<int>(n, 1));
	std::vector<std::vector<int>> B(n, std::vector<int>(n, 2));
	std::vector<std::vector<int>> C;
	for (auto _ : state) {
		C = multiply(A, B);
	}

	// Prevent the compiler from optimizing away the benchmark loop
	benchmark::DoNotOptimize(C);
}

// Function to multiply two matrices
std::vector<std::vector<int>> multiply_ijk(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B) {
	auto n = A.size();
	std::vector<std::vector<int>> C(n, std::vector<int>(n, 0));

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			for (int k = 0; k < n; k++) {
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}

	return C;
}
//BENCHMARK_CAPTURE(BM_MatrixMultiplication, multiply_ijk, multiply_ijk)->Arg(256)->Unit(benchmark::kMillisecond);

std::vector<std::vector<int>> multiply_ikj(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B) {
	auto n = A.size();
	std::vector<std::vector<int>> C(n, std::vector<int>(n, 0));

	for (int i = 0; i < n; i++) {
		for (int k = 0; k < n; k++) {
			for (int j = 0; j < n; j++) {
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}

	return C;
}
//BENCHMARK_CAPTURE(BM_MatrixMultiplication, multiply_ikj, multiply_ikj)->Arg(256)->Unit(benchmark::kMillisecond);

void BM_MatrixMultiply_blocking(benchmark::State& state, auto multiply) {
	std::vector<std::vector<int>> A(n, std::vector<int>(n, 1));
	std::vector<std::vector<int>> B(n, std::vector<int>(n, 2));
	std::vector<std::vector<int>> C;
	int blocking = int(state.range(0));

	for (auto _ : state) {
		C = multiply(A, B, blocking);
	}

	// Prevent the compiler from optimizing away the benchmark loop
	benchmark::DoNotOptimize(C);
}

std::vector<std::vector<int>> multiply_ijk_blocking(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, int blocking) {
	int n = int(A.size());
	std::vector<std::vector<int>> C(n, std::vector<int>(n, 0));

	int b = blocking;
	//for (int j = 0; j < n; j++) {
	for (int jj = 0; jj < n; jj += b)
		for (int kk = 0; kk < n; kk += b)
			for (int i = 0; i < n; i++)
				for (int j = jj; j < std::min(jj + b, n); j++) {
					auto r = 0;
					for (int k = kk; k < std::min(kk + b, n); k++)
						r += A[i][k] * B[k][j];
					C[i][j] += r;
				}
	return C;
}
//3 2 6 Reduce Miss Rate by Blocking https://www.youtube.com/watch?v=IDOB9lQrcyw
// if b is such that b*b submatrix will fit in a cache.
// 32Kb cache => we can have 4K 8byte doubles/integers => 4K = 64*64 matrix. => b = 64.
// b is called blocking factor. It is a parameter that can be tuned to get the best performance.
// blocking can improve special and temporal locality and reduce cache misses.
// for C =>  N/B * N/B *N * B = N^3/B cache misses.
// for A => N/B * N/B * B = N^3/B cache misses.
// for B => N/B * N/B * B * B = N^2 cache misses.

// total: 2*N^3/B + N^2 compulsory/capacity misses.
//before was (2N^3 + N^2) cache misses.
//
BENCHMARK_CAPTURE(BM_MatrixMultiply_blocking, multiply_ijk_blocking, multiply_ijk_blocking)->DenseRange(16, 33, 1)->DenseRange(90,120,1)->Unit(benchmark::kMillisecond);

}
