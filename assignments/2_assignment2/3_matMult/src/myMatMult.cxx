#include <iostream>
#include <opencv2/opencv.hpp>
#include <eigen3/Eigen/Dense>

void myMatMult(int rows, int cols, 
                float* mat1, float* mat2, float* matOut)
{ 
    int r = -cols;
    for (int i = 0; i < rows * cols; i++) {
        *(matOut + i) = 0.0f;
        if (i % cols==0) { r += cols; }
        for (int c = 0; c < cols; c++) {
            int aIdx = r + c;
            int bIdx = i%cols + rows*c;
            *(matOut + i) += *(mat1 + aIdx) * *(mat2 + bIdx);
        }
    }
}

cv::Mat ptrArrayToCvMat(int rows, int cols, float* mat){
    return cv::Mat(rows,cols,CV_32F,mat);
}

float* cvMatToPtrArray(cv::Mat cvMat){
    int s = cvMat.rows * cvMat.cols;
    cvMat = cvMat.reshape(1,s);
    return &(cvMat.at<float>(0,0));
}

Eigen::Matrix<float,-1,-1,Eigen::RowMajor> ptrArrayToEigenMat(int rows, int cols, float* mat){
    Eigen::Map<
        Eigen::Matrix<
            float,-1,-1,Eigen::RowMajor>>
                 eMat(mat,rows,cols);
    return eMat; 
}

float* eigenMatToPtrArray(Eigen::Matrix<float,-1,-1,Eigen::RowMajor> eMat){
    Eigen::Map<Eigen::RowVectorXf> eVec(eMat.data(), eMat.size());
    return &(eVec[0]);
}