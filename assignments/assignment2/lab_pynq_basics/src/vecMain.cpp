#include <iostream>
#include <stdlib.h>

#include <eigen3/Eigen/Dense>
// Or possibly just
// #include <Eigen/Dense>

#include "vecDot.h"
#include "vecCross.h"
#include "vecMulTest.h"

//using namespace Eigen;

using namespace std;

using Eigen::MatrixXd;
using Eigen::VectorXd;

int main(int argc, const char* argv[]) {
	MatrixXd m = MatrixXd::Random(3,3);
	m = (m * 10);
	cout << "m: " << endl << m << endl;
	VectorXd v(3);
	v << 1, 2, 3 ;
    cout << "v: " << endl << v << endl;
	cout <<"--Matric Vec Mult--\n" << "m*v: " << endl << m*v << endl;

    int len = 3;
    Eigen::VectorXf x(len);
    x << 1,2,3;
    Eigen::VectorXf y(len);
    y << 4,5,6; 

    printf("--Dot Product--\n");
	if (test_dot(x, y, len)) {
		printf("Test passed\n");
	}
	else {
		printf("Test failed\n");
		return 1;
	}
    cout << "x: \n" << x << endl;
    cout << "y: \n" << y << endl;
    cout << "dot(m,v): " << dot_product(x,y,len) << endl; 

    printf("--Cross Product--\n");
	if (test_cross(x, y)) {
		printf("Test passed\n");
	}
	else {
		printf("Test failed\n");
		return 1;
	}
    cout << "x: \n" << x << endl;
    cout << "y: \n" << y << endl;
    cout << "cross(m,v): " << cross_product_3d(x,y) << endl; 
	/*
		TODO-- Connect!

	// Test the methods
	*/
}

