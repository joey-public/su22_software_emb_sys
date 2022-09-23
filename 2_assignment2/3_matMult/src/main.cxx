#include <iostream>
#include <opencv2/opencv.hpp>
#include <eigen3/Eigen/Dense>
#include "myMatMult.h"
#include "cycletime.h"

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
    init_counters(1,1);
    const int ROWS = 3;
    const int COLS = 3;
    float a[ROWS][COLS] = { {1.0,2.0,3.0}, {4.0,5.0,6.0}, {7.0,8.0,9.0} };
    float b[ROWS][COLS] = { {1.0,2.0,1.0}, {3.0,1.0,7.0}, {5.0,3.0,5.3} };
    float customOut[ROWS][COLS] = { {0.0,0.0,0.0}, {0.0,0.0,0.0}, {0.0,0.0,0.0} };
    //Custom Naive 
    unsigned int c1 = get_cyclecount();
    myMatMult(ROWS, COLS, &a[0][0], &b[0][0], &(customOut[0][0]));
    unsigned int c2 = get_cyclecount();
    std::cout << "\nNaive Results: \n----------\n";
    std::cout << "Clock Cycles: " << c2-c1 << std::endl;
    printMat(ROWS,COLS,&(customOut[0][0])); 
    //OpenCv
    cv::Mat cvMatA = ptrArrayToCvMat(ROWS,COLS,&(a[0][0]));
    cv::Mat cvMatB = ptrArrayToCvMat(ROWS,COLS,&(b[0][0]));
    c1 = get_cyclecount();
    cv::Mat cvMatC = cvMatA*cvMatB;
    c2 = get_cyclecount();
    float* openCvOut = cvMatToPtrArray(cvMatC);
    std::cout << "\nOpenCv Results: \n----------\n";
    std::cout << "Clock Cycles: " << c2-c1 << std::endl;
    std::cout << cvMatC << std::endl;
    //Eigen
    Eigen::Matrix<float,-1,-1,Eigen::RowMajor> eigenMatA = ptrArrayToEigenMat(ROWS,COLS,&(a[0][0]));
    Eigen::Matrix<float,-1,-1,Eigen::RowMajor> eigenMatB = ptrArrayToEigenMat(ROWS,COLS,&(b[0][0]));
    c1 = get_cyclecount();
    Eigen::Matrix<float,-1,-1,Eigen::RowMajor> eigenMatC = eigenMatA*eigenMatB;
    c2 = get_cyclecount();
    float* eigenOut = eigenMatToPtrArray(eigenMatC);
    std::cout << "\nEigen Results: \n----------\n";
    std::cout << "Clock Cycles: " << c2-c1 << std::endl;
    std::cout << eigenMatC << std::endl;
	return 0;
}
