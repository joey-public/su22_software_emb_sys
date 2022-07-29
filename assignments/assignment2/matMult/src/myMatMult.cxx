#include <iostream>

void myMatMult(int rows, int cols, 
                float* matrixA, float* matrixB, float* matrixOut)
{ 
    for(int k=0; k<rows*cols; k++){
        int r = k / rows;
        int c = k % cols;
        for(int i=0; i<cols; i++){
            for(int j=0; j<rows; j++){
                std::cout << "[" << r << "][" << c << "]: ";
                std::cout << *(matrixB+k) << '\n';;
            }
        }
    }
}
