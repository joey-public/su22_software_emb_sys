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
    Eigen::Vector3f w(1.0f,2.0f,3.0f);
    Eigen::Vector3f z(4.0f,5.0f,6.0f);
    std::cout << w[0] << ',' << w[1] << ',' << w[2];

	if (test_cross(w, z)) {
		printf("Test passed\n");
	}
	else {
		printf("Test failed\n");
		return 1;
	}
    cout << "w: \n" << w << endl;
    cout << "z: \n" << z << endl;
    cout << "cross(m,v): " << cross_product(w,z) << endl; 
    printf("End\n");
}
