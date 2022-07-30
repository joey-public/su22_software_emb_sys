#include <iostream>
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
    float openCvOut[ROWS*COLS] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
    float eigneOut[ROWS][COLS] = { {0.0,0.0,0.0}, {0.0,0.0,0.0}, {0.0,0.0,0.0} };
    float* customOutPtr = &(customOut[0][0]);
    float* openCvOutPtr = &(openCvOut[0]);
    float* eigenOutPtr;
    myMatMult(ROWS, COLS, &a[0][0], &b[0][0], customOutPtr);
    printMat(ROWS, COLS, customOutPtr);
    openCvMatMult(ROWS, COLS, &a[0][0], &b[0][0], openCvOutPtr);
    std::cout << std::endl << openCvOutPtr<<std::endl;
    printMat(ROWS, COLS, openCvOutPtr);
	return 0;
}
