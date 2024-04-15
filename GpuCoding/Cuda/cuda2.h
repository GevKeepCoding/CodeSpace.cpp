#include "cuda.h"


void serialSimpleKernel(int N, float* d_a)
{
    for (int i = 0; i < N; i++)
    {
        d_a[i] = i;
    }
}


void tiledSerialSimpleKernel(int N, float* d_a) {
    for (blockIdx.x = 0; blockIdx.x < gridDim.x; blockIdx.x++) {
        for (threadIdx.x = 0; threadIdx.x < blockDim.x; threadIdx.x++) {
            const int n = threadIdx.x + blockIdx.x * blockDim.x;
            if (n < N) {
                d_a[n] = n;
            }
        }
    }
}

__global__ void SimpleKernel(int N, float* d_a) {
    const int n = threadIdx.x + blockIdx.x * blockDim.x;
    if (n < N) {
        d_a[n] = n;
    }
}

int main(){
    int N = 3789; // size of array
    float* d_a;
    cudaMalloc(&d_a, N*sizeof(float));


    int B = 512; // block size
    dim3 dimBlock(B,1,1); // 512 threads per block
    dim3 dimGrid((N+B-1)/b,1,1); // enough blocks to cover N

    // Queue kernel on Device
    simpleKernel<<<dimGrid, dimBlock>>>(d_a, N);

    // host arrray
    float* h_a = (float*)malloc(N*sizeof(float));
    // transfer result from device array to host array
    cudaMemcpy(h_a, d_a, N*sizeof(float), cudaMemcpyDeviceToHost);
    //print result
    for(int i=0; i<N; i++){
        printf("%f\n", h_a[i]);
    }
    // free memory
    free(h_a);
    cudaFree(d_a);
}

/*
Standard tree reduction at the thread-block level!!

Thread-block tree reduction in pseudo code:

t = thread index in  the thread block:
alive = number of threads in the thread block
s_sum[t] = u[global thread index];

while (alive > 1) {
    synchronize threads in thread-block;
    alive/=2;
    if (t < alive) {
        s_sum[t] += s_sum[t+alive];
    }
}

if (t == 0) block_sum[block  index] = s_sum[0];

*/
__global__ void partialReduceResidual(const int entries,
datafloat* u,
datafloat* newu, datafloat* blocksum)
{
    __shared__ datafloat s_blocksum[BDIM];
    const int id = blockIdx.x * blockDim.x + threadIdx.x;
    s_blocksum[threadIdx.x] = 0;

    if (id < entries) {
        const datafloat diff = u[id] - newu[id];
        s_blocksum[threadIdx.x] = diff * diff;
    }

    int alive = blockDim.x;
    int t = threadIdx.x;

    while (alive > 1) {
        __synchthreads();

        alive/=2;
        if (t < alive) {
            s_blocksum[t] += s_blocksum[t+alive];
        }
    }
    if (t == 0) {
        blocksum[blockIdx.x] = s_blocksum[0];
    }
}