#include "filter.h"
#include "timer.h"

#include <iostream>

#define KERNEL_SZ 3
int sobel_kernel_x[KERNEL_SZ][KERNEL_SZ] = {
	{ 1,  0, -1},
	{ 2,  0, -2},
	{ 1,  0, -1}};
int sobel_kernel_y[KERNEL_SZ][KERNEL_SZ] = {
	{ 1, 2, 1},
	{ 0, 0, 0},
	{-1, -2, -1}};

using namespace std;

// =================== Helper Functions ===================
inline int divup(int a, int b)
{
	if (a % b)
		return a / b + 1;
	else
		return a / b;
}

// =================== CPU Functions ===================
void sobel_filter_cpu(const uchar * input, uchar * output, const uint height, const uint width)
{
    float result_x, result_y, ac_x, ac_y;
    int idx;
    int offset = KERNEL_SZ/2;
    for(int i = 0; i < width*height; i++){
        result_x = 0.0f; result_y = 0.0f;
        idx = (i-width*offset)-offset;
        for(int r = 0; r < KERNEL_SZ; r++){
            for(int c = 0; c < KERNEL_SZ; c++){
                if(idx < 0 || idx > width*height){
                    ac_x = 0.0f; ac_y = 0.0f;
                }
                else{
                    ac_x = (float) input[idx] * (float) sobel_kernel_x[r][c];
                    ac_y = (float) input[idx] * (float) sobel_kernel_y[r][c];
                }
                result_x += ac_x;
                result_y += ac_y;
                idx += 1;                 
            }
            idx -= (KERNEL_SZ-1);
            idx += width;
        }
//        output[i] = (uchar) sqrt((result_x*result_x) + (result_y*result_y));
        output[i] = (uchar) ((result_x/2) + (result_y/2));
    }
}

// =================== GPU Kernel Functions ===================

// =================== GPU Host Functions ===================
void sobel_filter_gpu(const uchar * input, uchar * output, const uint height, const uint width)
{
}
