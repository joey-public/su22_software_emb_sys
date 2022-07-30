#include <iostream>
#include <opencv2/opencv.hpp>

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

/*
void openCvMatMult(int rows, int cols,  
                float* mat1, float* mat2, float*& matOut)
{
    cv::Mat a = cv::Mat(rows,cols,CV_32F,mat1);
    cv::Mat b = cv::Mat(rows,cols,CV_32F,mat2);
    cv::Mat c = (a*b);
    c.resize(1,rows*cols); 
    matOut = &(c.at<float>(0,0));
    std::cout << "\nOpenCV\n";
    std::cout << matOut << '\n';
    std::cout << &matOut << '\n';
    std::cout << "End OpenCv\n";
    a.resize(1,rows*cols);
    float* ptr = &(c.at<float>(0,0));
    for(int i=0; i<rows*cols; i++){
//        std::cout << c.at<float>(0,i) << ":" << *(ptr+i) <<'\n';
    }
}
*/

void eigenMatMult(int rows, int cols,  
                float* mat1, float* mat2, float* matOut)
{
}
