#include <iostream>
#include "sobel.h"

using namespace std;
using namespace cv;

const int KERNEL_SZ = 3;
int sobel_kernel_x[KERNEL_SZ][KERNEL_SZ] = {
	{ -1,  0, 1},
	{ -2,  0, 2},
	{ -1,  0, 1}};
int sobel_kernel_y[KERNEL_SZ][KERNEL_SZ] = {
	{ -1, -2, -1},
	{ 0, 0, 0},
	{1, 2, 1}};

void image_convolve(const Mat& src, Mat& dst, int kernel[KERNEL_SZ][KERNEL_SZ]){
    int i = 0;
    int result=0;
    int src_row, src_col, src_idx;
    unsigned char* dst_ptr = dst.ptr();
    for(int r= 1; r<= src.rows; r++){
        for(int c = 1; c <= src.cols; c++){
            result = 0;
            for(int kr=0; kr<KERNEL_SZ; kr++){
                for(int kc=0; kc<KERNEL_SZ; kc++){
                    src_row = (r-1)+kr;
                    src_col = (c-1)+kc;
                    result += kernel[kr][kc] * 
                                (int) src.at<unsigned char>(src_row, src_col);
                }
            }
            if(result > 255){result=255;}
            if(result < 0){result = 0;}
            *(dst_ptr+i) = result;
            i++;
        }
    }
    printf("done!\n");
}

void sobel(const Mat& src, Mat& dst)
{ 
    printf("Naive Sobel\n");
    std::cout << src << "\n--------------------\n";
    image_convolve(src,dst,sobel_kernel_x);
    image_convolve(src,dst,sobel_kernel_y);
    std::cout << "--------------------\n" << dst << '\n';
    printf("done!\n");
}

void sobel_unroll(const Mat& src, Mat& dst)
{
	// TODO
    printf("Unrolled Sobel\n");
}

void sobel_neon(const Mat& src, Mat& dst)
{
	// TODO
    printf("Neon Sobel\n");
}
