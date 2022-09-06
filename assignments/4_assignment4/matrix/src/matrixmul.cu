#include <stdio.h>
#include <stdlib.h>

#include "matrixmul.h"
#include "timer.h"

#define BLOCK_SIZE 32 

__global__ void block_mm_kernel(const float* A, const float* B, float* output, int M, int N) 
{
    const int tx=threadIdx.x, ty=threadIdx.y;
    const int bx=blockIdx.x, by=blockIdx.y;
    const int dim=blockDim.x;
    const int row = blockDim.y * by + ty; 
    const int col = blockDim.x * bx + tx; 

    if(row > N || col > N) return;
    __shared__ float aSub[BLOCK_SIZE*BLOCK_SIZE];
    __shared__ float bSub[BLOCK_SIZE*BLOCK_SIZE];
    int cIdx = row*N+col; 
    float cValue = 0.0f;
    for(int i=0; i<M/dim; i++)
    {
       aSub[ty*dim+tx] = A[(row*M) + (i*dim) + tx]; 
       bSub[ty*dim+tx] = B[(i*dim*N) + (ty*N) + col]; 
       __syncthreads();
       for(int j=0;j<BLOCK_SIZE;j++)
       {
            cValue += aSub[ty*dim+j] * bSub[j*dim+tx];
       }
       __syncthreads();
    }
    output[cIdx] = cValue;
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
    dim3 gridD(divup(N,BLOCK_SIZE), divup(N,BLOCK_SIZE), 1);
    dim3 blockD(BLOCK_SIZE, BLOCK_SIZE, 1);
    block_mm_kernel<<<gridD,blockD>>>(A,B,C,M,N);
	CudaCheckError();
	CudaSafeCall(cudaDeviceSynchronize());
	gpu_timer.stop();
	float gpu_time = gpu_timer.getElapsed();
	gpu_timer.end();
	return gpu_time;
}
