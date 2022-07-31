#ifndef MYMATMULT_H
#define MYMATMULT_H

void myMatMult(int rows, int cols, 
                float* matrixA, float* matrixB, 
                float* matrixOut);

cv::Mat ptrArrayToCvMat(int rows, int cols, float* mat);
float* cvMatToPtrArray(cv::Mat cvMat);

Eigen::Matrix<float,-1,-1,Eigen::RowMajor> ptrArrayToEigenMat(int rows, int cols, float* mat);
float* eigenMatToPtrArray(Eigen::Matrix<float,-1,-1,Eigen::RowMajor> eMat);
#endif
