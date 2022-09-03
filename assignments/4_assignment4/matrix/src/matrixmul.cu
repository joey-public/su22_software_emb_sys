#include <stdio.h>
#include <stdlib.h>

#include "matrixmul.h"
#include "timer.h"

#define BLOCK_SIZE 16

__global__ void block_mm_kernel(const float* A, const float* B, float* output, int M, int N, const int block_size) 
{
    const int block = blockIdx.x;
    const int thread = threadIdx.x;
    int idx_a, idx_b, idx_out;
    idx_out = block*block_size + thread;
    idx_a = (blockDim.x / block)*block_size*M;
    idx_b = (blockDim.x / block)*block_size*N;
    output[idx_out]=0;
    for(int i = 0; i<block_size; i++){//bs # of dot products
        for(int j=0; j<block_size; j++){//do the dot product
            output[idx_out] += A[idx_a]*B[idx_b]; 
            idx_a += 1;
            idx_a += block_size;
        }
        idx_a += 1;
        idx_b += N;
    }
}


inline int divup(int a, int b)
{
	if (a % b)
		return a / b + 1;
	else
		return a / b;
}


float run_mm_gpu(const float* A, const float* B, float* C, int M, int N)
{
	Timer gpu_timer;
	gpu_timer.start();
    int blocks = (N/BLOCK_SIZE) * (N/BLOCK_SIZE);
    int threads = BLOCK_SIZE*BLOCK_SIZE;
    block_mm_kernel<<<(blocks,1,1),(threads,1,1)>>>(A,B,C,M,N,BLOCK_SIZE);
	CudaCheckError();
	CudaSafeCall(cudaDeviceSynchronize());
	gpu_timer.stop();
	float gpu_time = gpu_timer.getElapsed();
	gpu_timer.end();
	return gpu_time;
}
