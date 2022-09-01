#include <stdio.h>
#include <stdlib.h>

#include "matrixmul.h"
#include "timer.h"

#define BLOCK_SIZE 16

__global__ void block_mm_kernel(const float* A, const float* B, float* output, int M, int N) 
{
	// TODO: complete the block matrix kernel function
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

	//TODO: launch the kernel function
	
	CudaCheckError();
	CudaSafeCall(cudaDeviceSynchronize());
	gpu_timer.stop();
	float gpu_time = gpu_timer.getElapsed();
	gpu_timer.end();

	return gpu_time;
}


