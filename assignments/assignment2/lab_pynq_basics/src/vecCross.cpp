#include "vecCross.h"

Eigen::VectorXf cross_product_3d(Eigen::VectorXf vec1, Eigen::VectorXf vec2) {
	// Note: This function does not use Eigen; it simply uses the Eigen
	// vector type to keep this example simple. A STL vector would work the
	// same for the implementation approach.

    float a = vec1[1]*vec2[2]-vec1[2]*vec2[1]; 
    float b = vec1[2]*vec2[0]-vec1[0]*vec2[2]; 
    float c = vec1[0]*vec2[1]-vec1[1]*vec2[0]; 
	Eigen::VectorXf result(3);
    result << a,b,c;
	return result;
}
