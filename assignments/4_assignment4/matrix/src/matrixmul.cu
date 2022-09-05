#include <stdio.h>
#include <stdlib.h>

#include "matrixmul.h"
#include "timer.h"

#define BLOCK_SIZE 2 

__global__ void block_mm_kernel(const float* A, const float* B, float* output, int M, int N, const int block_size) 
{
    const int x = blockIdx.x * blockDim.x + threadIdx.x;
    const int y = blockIdx.y * blockDim.y + threadIdx.y;
    if(x>N || y>N) { return; }
    int a_idx = y*N;
    int b_idx = x;
    int c_idx = y*N+x;
    output[c_idx] = 0.0f;
    for(int i=0; i<N/blockDim.x; i++)
    {
        output[c_idx] += A[a_idx]*B[b_idx];
        a_idx += 1;
        b_idx += N;
    }
    __syncthreads();
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
    dim3 blocks(N/BLOCK_SIZE,N/BLOCK_SIZE,1);
    dim3 threads(BLOCK_SIZE,BLOCK_SIZE,1);
    block_mm_kernel<<<blocks,threads>>>(A,B,C,M,N,BLOCK_SIZE);
	CudaCheckError();
	CudaSafeCall(cudaDeviceSynchronize());
	gpu_timer.stop();
	float gpu_time = gpu_timer.getElapsed();
	gpu_timer.end();
	return gpu_time;
}
