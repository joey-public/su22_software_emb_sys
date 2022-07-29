#include <iostream>
#include "myMatMult.h"

int main(){
    const int ROWS = 2;
    const int COLS = 2;
    float mat1[][COLS] = { {1.0f, 0.0f}, 
                               {0.0f, 1.0f} };
    float mat2[][COLS] = { {1.4f, 2.2f},
                               {3.0f, 4.2f} };
    float result[ROWS][COLS];
    myMatMult(ROWS, COLS, &mat1[0][0],
                          &mat2[0][0],
                          &result[0][0]);
}
