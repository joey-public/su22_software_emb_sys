#ifndef MYMATMULT_H
#define MYMATMULT_H

void myMatMult(int rows, int cols, 
                float* matrixA, float* matrixB, 
                float* matrixOut);

cv::Mat ptrArrayToCvMat(int rows, int cols, float* mat);
float* cvMatToPtrArray(cv::Mat cvMat);
void openCvMatMult(int rows, int cols,  
                float* mat1, float* mat2, float* matOut);

void eigenMatMult(int rows, int cols,  
                float* mat1, float* mat2, float* matOut);
#endif
