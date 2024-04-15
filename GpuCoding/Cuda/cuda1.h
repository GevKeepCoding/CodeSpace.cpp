// How GPU Computing Works | GTC 2021   https://www.youtube.com/watch?v=3l10o0DYJXg

void daxpy(int n, double alpha, double *x, double *y)
{
    for (int i = 0; i < n; i++)
    {
        y[i] = alpha * x[i] + y[i];
    }
}


/*
1 inch = 2.54 cm
1 foot = 12 inches
1 cm = 10 mm
In 1 clock tick light travels 100mm (~4 inches)
in 1 clock tick electricity travels 20mm (~0.8 inches)

Ampere A100 GPU
108 SMs
256Kb register File per SM(27MB total)
192KB L1 cache & shared memory per SM(20MB total)
40MB L2 cache shared across all SMs

                  Bandwidth Latency
L1                13x       1x
L2                3x        5x
HBM Memory(80GB)  1x        50x

PCIe Off Chip     0.02x     25x

Data location     Bandwidth(GB/sec)  (Compute Intensity)
L1 Cache          19,400              8
L2 Cache          4,000               39
HBM Memory        1,555               100
NVLINK            300                 520
PCIe              25                  6240

Data location  Latency(ns) Threads Required
L1 Cache       27          32,738
L2 Cache       150         37,500
HBM Memory     404         39,264
NVLINK         700         13,125
PCIe           1470        2297



*/












// How CUDA Programming Works | GTC 2022  https://www.youtube.com/watch?v=n6M8R8-PlnE

#include <cmath>
#include <cuda_runtime.h>


__global__ void euclidian_distance(float2* p1, float2* p2, float* distance, int count)
{
    int index = threadIdx.x + (blockIdx.x * blockDim.x);

    if (index < count) {
        float2 dp = p2[index]- p1[index];
        float dist = sqrtf(dp.x*dp.x + dp.y*dp.y);

        distance[index]=dist;
    }
}
/*
For A100 gpu
1 page is == 1024 bytes.
1 warp has 32 threads.

A block of threads is broken up into "warps" of 32 threads A "warp" is the vector element of GPU.

The SM(streaming multiprocessor) can manage up to 64 warps of thread at the same time,
but at any given time it may run 4 warps.

1warp = (32threads) * 8 bytes(2floats) = 256 bytes.
1SM has 4warps => 4 warps * 256 bytes pwer warp = 1024bytes (it a page size and is the perfect size to fetch)

This gpu has 1555 GB/sec memory bandwidth and 108 SM.
Total threads == 221,184
Peak FP32 TFLOPS/s = 19.5
Peak FP64 TFLOPS/s(non-tensor) == 9.7
Peak FP64 TFLOPS/s = 19.5

Shared Memory per SM = 160KB
L2 Cache Size = 40960KB
GPU Boost Clock = 1420 MHz


*/

