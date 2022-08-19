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
//    cout << dst << endl;
    printf("\ndone!\n");
}

void sobel_unroll(const Mat& src, Mat& dst)
{
    printf("Unrolled Sobel\n");
    const int rows = src.rows;
    const int cols = src.cols;
    Mat region = Mat(3,3,CV_8UC1);
    float ans[rows][cols];
//    cout << src << endl;
    for (int r = 1; r < rows-1; r++){
        for(int c = 1; c < cols-1; c++){
            float result_x[4]={0.0};
            float result_y[4]={0.0};
            Rect rect = Rect(c-1,r-1,3,3);
            region = src(rect).clone();
//            cout << "r: " << r << " c: " << c << endl;
//            cout << region << endl;
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
/*
                cout << "i = " << i << endl; 
                cout << "img: ";
                cout << (float) img[i+0] << ", ";
                cout << (float) img[i+1] << ", ";
                cout << (float) img[i+2] << ", ";
                cout << (float) img[i+3] << endl;
                cout << "knx: ";
                cout << (float) kernx[i+0] << ", ";
                cout << (float) kernx[i+1] << ", ";
                cout << (float) kernx[i+2] << ", ";
                cout << (float) kernx[i+3] << endl;
                cout << "kny: ";
                cout << (float) kerny[i+0] << ", ";
                cout << (float) kerny[i+1] << ", ";
                cout << (float) kerny[i+2] << ", ";
                cout << (float) kerny[i+3] << endl;
                cout << "rsx: ";
                cout << result_x[0] << ", ";
                cout << result_x[1] << ", ";
                cout << result_x[2] << ", ";
                cout << result_x[3] << endl;
                cout << "rsy: ";
                cout << result_y[0] << ", ";
                cout << result_y[1] << ", ";
                cout << result_y[2] << ", ";
                cout << result_y[3] << endl;
*/
            }
            float x = result_x[0]+result_x[1]+result_x[2]+result_x[3]; 
            x += (float) kernx[8] * (float) img[8];
            float y = result_y[0]+result_y[1]+result_y[2]+result_y[3]; 
            y += (float) kerny[8] * (float) img[8];
//            cout << "x: " << x << "y: " << y << endl;
            ans[r][c] = sqrt( (x*x)+(y*y) );
        }
    } 
    dst = Mat(rows,cols,CV_32FC1,ans);
    dst.convertTo(dst, CV_8U);
    printf("\ndone!\n");
}

void sobel_neon(const Mat& src, Mat& dst)
{
    printf("Unrolled Sobel\n");
    const int rows = src.rows;
    const int cols = src.cols;
    Mat region = Mat(3,3,CV_8UC1);
    float ans[rows][cols];
    float32x4_t result_x;
    float32x4_t result_y;
    float32x4_t img_reg;
    float32x4_t kernel_x;
    float32x4_t kernel_y;
    for (int r = 1; r < rows-1; r++){
        for(int c = 1; c < cols-1; c++){
            region = src(Rect(c-1,r-1,3,3)).clone();
            region.convertTo(region, CV_32FC1);
            cout << region.ptr()[10] << endl;
            unsigned char* img = region.ptr();
            float* kernx = &neon_sobel_kernel_x[0][0];
            float* kerny = &neon_sobel_kernel_y[0][0];
            result_x = vmovq_n_f32(0);
            result_y = vmovq_n_f32(0);
/*
            for(int i=0; i<8; i+=4){
                img_reg = vld1q_f32(&img[i]); 
                kernel_x = vld1q_f32(&kernx[i]);
                kernel_y = vld1q_f32(&kerny[i]);
                result_x = vaddq_f32(result_x, vmulq_f32(img_reg,kernel_x));
                result_y = vaddq_f32(result_y, vmulq_f32(img_reg,kernel_y));
            }    
*/
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

/*
void sobel_neon(const Mat& src, Mat& dst)
{
    printf("Neon Sobel\n");
    const int rows = src.rows;
    const int cols = src.cols;
    float ans[rows][cols];
    float32x4_t result_x;
    float32x4_t result_y;
    float32x4_t img_reg;
    float32x4_t kernel_x;
    float32x4_t kernel_y;
    for (int r = 1; r < rows-1; r++){
        for(int c = 1; c < cols-1; c++){
            Rect rect = Rect(r-1,c-1,3,3);
            unsigned char* img = src(rect).ptr();
            result_x = vmovq_n_f32(0);
            result_y = vmovq_n_f32(0);
            for(int i = 0; i < 8; i+=4){
                float temp = (float) img[i];
                float kernx = (float) *sobel_kernel_x[i];
                float kerny = (float) *sobel_kernel_y[i];
                img_reg = vld1q_f32(&temp);
                kernel_x = vld1q_f32(&kernx);
                kernel_y = vld1q_f32(&kerny);
                result_x = vaddq_f32(result_x, vmulq_f32(img_reg,kernel_x));
                result_y = vaddq_f32(result_y, vmulq_f32(img_reg,kernel_y));
            }
            float x = result_x[0]+result_x[1]+result_x[2]+result_x[3]; 
            x += (float) *sobel_kernel_x[8] * (float) img[8];
            float y = result_y[0]+result_y[1]+result_y[2]+result_y[3]; 
            y += (float) *sobel_kernel_y[8] * (float) img[8];
            ans[r][c] = sqrt( (x*x)+(y*y) );
//            float32_t x = vaddvq_f32(result_x); 
//            float32_t y = vaddvq_f32(result_y); 
        }
    } 
    dst = Mat(rows,cols,CV_32FC1,ans);
    dst.convertTo(dst, CV_8U);
    printf("\ndone!\n");
}
*/
