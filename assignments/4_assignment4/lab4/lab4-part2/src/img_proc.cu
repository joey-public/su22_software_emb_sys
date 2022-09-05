#include "img_proc.h"
#include <cstdio> 

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
    for(int i = 0; i < width*height; i++){
        out[i] = 255-in[i];     
    }     
    
}

void img_blur_cpu(uchar* out, const uchar* in, const uint width, const uint height, const int blur_size)
{
    float scale = 1/blur_size;
    int offset = blur_size/2;
    for(int i = 0; i < width*height; i++){
        int result = 0;
        int ac = 0;
        int idx = (i-width*offset)-offset;
        for(int j = 0; j<blur_size; j++){
            for(int j =0; j<blur_size; j++){
                if(idx < 0 || idx > width*height){
                    ac = 0;
                }
                else{
                    ac = in[idx];
                }
                result += ac;
                idx+=1;
            }
            idx -= (blur_size-1);
            idx += width;
        }
        out[i] = result / (blur_size*blur_size);
    }
}

// =================== GPU Kernel Functions ===================
/*
TODO: Write GPU kernel functions for the above functions
   */
__global__ void kernel_img_rgb2gray(uchar* out, const uchar* in, const uint width, const uint height, const int channels)
{
    const int x = blockIdx.x * blockDim.x + threadIdx.x; 
    const int y = blockIdx.y * blockDim.y + threadIdx.y; 
    int out_idx, in_idx;
    if(x < width && y < height){
        out_idx = y*width + x;
        in_idx = out_idx * channels;
        uchar r = in[in_idx];
        uchar g = in[in_idx+1];
        uchar b = in[in_idx+2];
        uchar gray_pixel = (r+g+b)/channels;
        out[out_idx] = gray_pixel;
    }
}

__global__ void kernel_img_invert(uchar* out, const uchar* in, const uint width, const uint height)
{
    const int x = blockIdx.x * blockDim.x + threadIdx.x; 
    const int y = blockIdx.y * blockDim.y + threadIdx.y; 
    int idx;
    if(x < width && y < height){
        idx = y*width + x;
        out[idx] = 255-in[idx];
    }
}

__global__ void kernel_img_blur(uchar* out, const uchar* in, const uint width, const uint height, const int blur_size)
{
    const int x = blockIdx.x * blockDim.x + threadIdx.x; 
    const int y = blockIdx.y * blockDim.y + threadIdx.y; 
    int offset = blur_size/2;
    int i;
    if(x < width && y < height){
        i = y*width + x;
        int result = 0;
        int ac = 0;
        int idx = (i-width*offset)-offset;
        for(int j = 0; j<blur_size; j++){
            for(int j =0; j<blur_size; j++){
                if(idx < 0 || idx > width*height){
                    ac = 0;
                }
                else{
                    ac = in[idx];
                }
                result += ac;
                idx+=1;
            }
            idx -= (blur_size-1);
            idx += width;
        }
        out[i] = result / (blur_size*blur_size);
    }
}

// =================== GPU Host Functions ===================
/* 
TODO: Write GPU host functions that launch the kernel functions above
   */
void img_rgb2gray(uchar* out, const uchar* in, const uint width, const uint height, const int channels)
{
    const int grid_x = 64;
    const int grid_y = 64;
    dim3 grid(grid_x, grid_y, 1);
    dim3 block(divup(width, grid_x), divup(height, grid_y), 1);
    kernel_img_rgb2gray<<<grid,block>>>(out, in, width, height, channels);
    cudaDeviceSynchronize(); 
}

void img_invert(uchar* out, const uchar* in, const uint width, const uint height)
{
    const int grid_x = 64;
    const int grid_y = 64;
    dim3 grid(grid_x, grid_y, 1);
    dim3 block(divup(width, grid_x), divup(height, grid_y), 1);
    kernel_img_invert<<<grid,block>>>(out, in, width, height);
    cudaDeviceSynchronize(); 
}

void img_blur(uchar* out, const uchar* in, const uint width, const uint height, const int blur_size)
{
    const int grid_x = 64;
    const int grid_y = 64;
    dim3 grid(grid_x, grid_y, 1);
    dim3 block(divup(width, grid_x), divup(height, grid_y), 1);
    kernel_img_blur<<<grid,block>>>(out, in, width, height, blur_size);
    cudaDeviceSynchronize(); 
}
