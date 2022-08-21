#include <iostream>
#include <arm_neon.h>
#include "sobel.h"

using namespace std;
using namespace cv;

const int KERNEL_SZ = 3;
int sobel_kernel_x[KERNEL_SZ][KERNEL_SZ] = {
	{ 1,  0, -1},
	{ 2,  0, -2},
	{ 1,  0, -1}};
int sobel_kernel_y[KERNEL_SZ][KERNEL_SZ] = {
	{ 1, 2, 1},
	{ 0, 0, 0},
	{-1, -2, -1}};
float neon_sobel_kernel_x[KERNEL_SZ][KERNEL_SZ] = {
	{ 1.0f,  0.0f, -1.0f},
	{ 2.0f,  0.0f, -2.0f},
	{ 1.0f,  0.0f, -1.0f}};
float neon_sobel_kernel_y[KERNEL_SZ][KERNEL_SZ] = {
	{ 1.0f, 2.0f, 1.0f},
	{ 0.0f, 0.0f, 0.0f},
	{-1.0f, -2.0f, -1.0f}};

void sobel(const Mat& src, Mat& dst)
{ 
    printf("Naive Sobel\n");
    //1. calculate s_x and s_y matricies
    //2. calcultate s_x_abs and s_y_abs
    //3. calcultate mag_squared
    //4. calculate the  
    const int rows = src.rows;
    const int cols = src.cols;
    float result_x, result_y;
    float ans[rows][cols];
    for (int r = 1; r < rows-1; r++){
        for(int c = 1; c < cols-1; c++){
            result_x=0; result_y=0; 
            for(int i = 0; i < KERNEL_SZ; i++){
                for(int j = 0; j < KERNEL_SZ; j++){
                    result_x += (float) sobel_kernel_x[i][j] * (float) src.at<unsigned char>(r-1+i,c-1+j);
                    result_y += (float) sobel_kernel_y[i][j] * (float) src.at<unsigned char>(r-1+i,c-1+j);
                }
            }
        ans[r][c] = sqrt((result_x*result_x) + (result_y*result_y));
        }
    } 
    dst = Mat(rows,cols,CV_32FC1,ans);
    dst.convertTo(dst, CV_8U);
    printf("\ndone!\n");
}

void sobel_unroll(const Mat& src, Mat& dst)
{
    printf("Unrolled Sobel\n");
    const int rows = src.rows;
    const int cols = src.cols;
    Mat region = Mat(3,3,CV_8UC1);
    float ans[rows][cols];
    for (int r = 1; r < rows-1; r++){
        for(int c = 1; c < cols-1; c++){
            float result_x[4]={0.0};
            float result_y[4]={0.0};
            Rect rect = Rect(c-1,r-1,3,3);
            region = src(rect).clone();
            unsigned char* img = region.ptr();
            int* kernx = &sobel_kernel_x[0][0];
            int* kerny = &sobel_kernel_y[0][0];
            for(int i=0; i<8; i+=4){
                result_x[0] += (float) img[i+0] * (float) kernx[i+0];
                result_x[1] += (float) img[i+1] * (float) kernx[i+1];
                result_x[2] += (float) img[i+2] * (float) kernx[i+2];
                result_x[3] += (float) img[i+3] * (float) kernx[i+3];
                result_y[0] += (float) img[i+0] * (float) kerny[i+0];
                result_y[1] += (float) img[i+1] * (float) kerny[i+1];
                result_y[2] += (float) img[i+2] * (float) kerny[i+2];
                result_y[3] += (float) img[i+3] * (float) kerny[i+3];
            }
            float x = result_x[0]+result_x[1]+result_x[2]+result_x[3]; 
            x += (float) kernx[8] * (float) img[8];
            float y = result_y[0]+result_y[1]+result_y[2]+result_y[3]; 
            y += (float) kerny[8] * (float) img[8];
            ans[r][c] = sqrt( (x*x)+(y*y) );
        }
    } 
    dst = Mat(rows,cols,CV_32FC1,ans);
    dst.convertTo(dst, CV_8U);
    printf("\ndone!\n");
}

void sobel_neon(const Mat& src, Mat& dst)
{
    printf("Neon Sobel\n");
    const int rows = src.rows;
    const int cols = src.cols;
    Mat region = Mat(3,3,CV_8UC1);
    Mat region_float = Mat(3,3,CV_8UC1);
    float ans[rows][cols];
    float32x4_t result_x;
    float32x4_t result_y;
    float32x4_t img_reg;
    float32x4_t kernel_x;
    float32x4_t kernel_y;
    for (int r = 1; r < rows-1; r++){
        for(int c = 1; c < cols-1; c++){
            region = src(Rect(c-1,r-1,3,3)).clone();
            region.convertTo(region_float, CV_32FC1);
            float* img = &region_float.at<float>(0,0);
            float* kernx = &neon_sobel_kernel_x[0][0];
            float* kerny = &neon_sobel_kernel_y[0][0];
            result_x = vmovq_n_f32(0);
            result_y = vmovq_n_f32(0);
            for(int i=0; i<8; i+=4){
                img_reg = vld1q_f32(&img[i]); 
                kernel_x = vld1q_f32(&kernx[i]);
                kernel_y = vld1q_f32(&kerny[i]);
                result_x = vaddq_f32(result_x, vmulq_f32(img_reg,kernel_x));
                result_y = vaddq_f32(result_y, vmulq_f32(img_reg,kernel_y));
            }    
            float x = result_x[0]+result_x[1]+result_x[2]+result_x[3]; 
            x += kernx[8] * img[8];
            float y = result_y[0]+result_y[1]+result_y[2]+result_y[3]; 
            y += kerny[8] * img[8];
            ans[r][c] = sqrt( (x*x)+(y*y) );
        }
    } 
    dst = Mat(rows,cols,CV_32FC1,ans);
    dst.convertTo(dst, CV_8U);
    printf("\ndone!\n");
}

void sobel_opencv(const Mat& src, Mat& dst){
    printf("\nSobel OpenCV\n");
    Mat s_x, s_y, cv_sobel_out;
    Mat s_x_abs, s_y_abs, mag_squared;
    Sobel(src, s_x, CV_32F, 1, 0, 3, 1, 0, BORDER_ISOLATED);
    Sobel(src, s_y, CV_32F, 0, 1, 3, 1, 0, BORDER_ISOLATED);
    pow(s_x, 2, s_x_abs);
    pow(s_y, 2, s_y_abs);
    add(s_x_abs , s_y_abs, mag_squared);
    sqrt(mag_squared, cv_sobel_out);
    cv_sobel_out.convertTo(dst, CV_8U);
    printf("\ndone!\n");
}
