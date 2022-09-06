#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>

#include <opencv2/imgproc/imgproc.hpp>
#include <cuda_runtime.h>

#include "matrixmul.h"
#include "timer.h"

//#define DEBUG

using namespace std;
using namespace cv;

#ifdef DEBUG
void printMat(float*m, int r, int c){
    for(int i = 0; i < r; i++){
        for(int j = 0; j < c; j++){
            std::cout << m[i*c+j] << " | ";
        }
        std::cout << std::endl;
    }
}
#endif

// ***********************************************
int main(int argc, char const *argv[])
// ***********************************************
{
	if(argc < 3)
	{
		cout << "Usage: " << argv[0] << " M N" << endl;
		return 1;
	}

	int M = atoi(argv[1]);
	int N = atoi(argv[2]);

	srand(time(0));

	float* h_A;
	float* h_B;
	float* h_C;
	float* h_Ccpu;
//    printf("Allocating GPU MEM\n");
	//TODO: allocate the unified memory for the input/output matrices. The program will result in a segfault until you complete this line.
    cudaMallocManaged(&h_A, N*M*sizeof(float));
    cudaMallocManaged(&h_B, M*N*sizeof(float));
    cudaMallocManaged(&h_C, N*N*sizeof(float));

	// Initialize matrices
//    printf("Init A\n");
	for(int i = 0; i < N; i++)
	{
		for(int j = 0; j < M; j++)
		{
			h_A[i * M + j] = rand() / (float)RAND_MAX;
		}
	}
//    printf("Init B\n");
	for(int i = 0; i < M; i++)
	{
		for(int j = 0; j < N; j++)
		{
			h_B[i * N + j] = rand() / (float)RAND_MAX;
		}
	}

	// MM GPU
	float time_gpu = -1.f;
//    printf("Call gpu mm func\n");
    time_gpu = run_mm_gpu(h_A, h_B, h_C, M, N);

	// Profiling
	float time_cpu;

	Timer cpu_timer;
	cpu_timer.start();

	Mat cv_A = Mat(N, M, CV_32F, h_A);
//    printf("Calc with OPCV\n");
	Mat cv_B = Mat(M, N, CV_32F, h_B);
//    printf("Calc with OPCVb\n");
	Mat cv_C = Mat(N, N, CV_32F);
//    printf("Calc with OPCVc\n");

	cv_C = cv_A * cv_B;

	cpu_timer.stop();
	time_cpu = cpu_timer.getElapsed();
	cpu_timer.end();

#ifdef DEBUG
    std::cout << endl << "A:\n";
    printMat(h_A,N,M);
    std::cout << endl << "B:\n";
    printMat(h_B,M,N);
    std::cout << endl << "C Cuda:\n";
    printMat(h_C,N,N);
    std::cout << endl << "C OpenCv:\n";
    printMat(cv_C.ptr<float>(),N,N);
    std::cout << endl;
#endif

	// Check for errors
	float mse = 0.f;
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			float diff = abs(h_C[i*N + j] - cv_C.ptr<float>()[i*N + j]);
			mse += diff * diff;
		}
	}
	mse /= N*N;

	float rmse = sqrt(mse);

	stringstream ss;
	ss << fixed;
	ss << setprecision(2);
	ss << "Time CPU = " << time_cpu << "ms, Time GPU = " << time_gpu << "ms, Speedup = " << time_cpu/time_gpu << "x, RMSE = ";
	ss << setprecision(5);
	ss << rmse;
    
    cout << "Running for " << M << "x" << N << endl;
	cout << ss.str() << endl;
    cout << "---------------------------------------------------------------------\n\n";

	// Free memory
	//TODO: free the device memory
    cudaFree(h_A);
    cudaFree(h_B);
    cudaFree(h_C);


	return 0;
}
