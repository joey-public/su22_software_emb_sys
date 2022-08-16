#include <iostream>
#include "sobel.h"

using namespace std;
using namespace cv;

const int KERNEL_SZ = 3;
int sobel_kernel_x[KERNEL_SZ][KERNEL_SZ] = {
	{ 1,  0, -1},
	{ 2,  0, -2},
	{ 1,  0, -1}};

int sobel_kernel_y[KERNEL_SZ][KERNEL_SZ] = {
	{ -1, -2, -1},
	{ 0, 0, 0},
	{1, 2, 1}};

void sobel(const Mat& src, Mat& dst)
{ 
    printf("Naive Sobel\n");
    //1. calculate s_x and s_y matricies
    //2. calcultate s_x_abs and s_y_abs
    //3. calcultate mag_squared
    //4. calculate the  
    const int rows = src.rows;
    const int cols = src.cols;
    int result_x, result_y;
    int ans[rows][cols];
    for (int r = 1; r < rows-1; r++){
        for(int c = 1; c < cols-1; c++){
            result_x=0; result_y=0; 
            for(int i = 0; i < KERNEL_SZ; i++){
                for(int j = 0; j < KERNEL_SZ; j++){
                    result_x += sobel_kernel_x[i][j] * (int) src.at<unsigned char>(r-1+i,c-1+j);
                    result_y += sobel_kernel_y[i][j] * (int) src.at<unsigned char>(r-1+i,c-1+j);
                }
            }
        ans[r][c] = sqrt((result_x*result_x) + (result_y*result_y));
        }
    } 
    dst = Mat(rows,cols,CV_32SC1,ans);
    dst.convertTo(dst, CV_8U);
    cout << dst << endl;
    printf("\ndone!\n");
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
