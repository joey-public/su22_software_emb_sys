#include <iostream>
#include <opencv2/opencv.hpp>
#include "myMatMult.h"

void printMat(int rows, int cols, float* mat) {
    for (int i = 0; i < rows * cols; i++) {
        std::cout << *(mat + i) << " , ";
        if ((i+1) % cols == 0) { std::cout << std::endl; }
    }
    std::cout << "-----" << std::endl;
}
bool equal(int rows, int cols, float* mat1, float* mat2){
    for (int i = 0; i < rows * cols; i++) {
        if (*(mat1+i) != *(mat2+i)) { return false; }
    }
    return true;
}

int main(){
    const int ROWS = 3;
    const int COLS = 3;
    float a[ROWS][COLS] = { {1.0,2.0,3.0}, {4.0,5.0,6.0}, {7.0,8.0,9.0} };
    float b[ROWS][COLS] = { {1.0,2.0,1.0}, {3.0,1.0,7.0}, {5.0,3.0,5.3} };
    float customOut[ROWS][COLS] = { {0.0,0.0,0.0}, {0.0,0.0,0.0}, {0.0,0.0,0.0} };
    myMatMult(ROWS, COLS, &a[0][0], &b[0][0], &(customOut[0][0]));
    //OpenCv
    cv::Mat cvMatA = ptrArrayToCvMat(ROWS,COLS,&(a[0][0]));
    cv::Mat cvMatB = ptrArrayToCvMat(ROWS,COLS,&(b[0][0]));
    cv::Mat cvMatC = cvMatA*cvMatB;
    float* openCvOut = cvMatToPtrArray(cvMatC);
    for(int i=0; i < 9; i++){
        std::cout << openCvOut+i << ":" << *(openCvOut + i) << '\n';
    }
    //Eigen
	return 0;
}
