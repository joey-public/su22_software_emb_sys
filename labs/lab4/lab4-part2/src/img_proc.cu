#include "img_proc.h"

// =================== Helper Functions ===================
inline int divup(int a, int b)
{
	if (a % b)
		return a / b + 1;
	else
		return a / b;
}

// =================== CPU Functions ===================

void img_rgb2gray_cpu(uchar* out, const uchar* in, const uint width, const uint height, const int channels)
{
    int out_idx = 0;
    int avg = 0;
    for(int i = 0; i < channels*width*height; i++){
        avg += in[i];
        if((i+1)%channels == 0){
            avg = avg / channels;
            out[out_idx] = avg; 
            avg = 0;
            out_idx++; 
        }     
    }
}

void img_invert_cpu(uchar* out, const uchar* in, const uint width, const uint height)
{
    //TODO: Invert a 8bit image
}

void img_blur_cpu(uchar* out, const uchar* in, const uint width, const uint height, const int blur_size)
{
    //TODO: Average out blur_size pixels
}

// =================== GPU Kernel Functions ===================
/*
TODO: Write GPU kernel functions for the above functions
   */

// =================== GPU Host Functions ===================
/* 
TODO: Write GPU host functions that launch the kernel functions above
   */
