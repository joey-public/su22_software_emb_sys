#include "vecCross.h"

Eigen::Vector3f cross_product(Eigen::Vector3f vec1, Eigen::Vector3f vec2) {
	// Note: This function does not use Eigen; it simply uses the Eigen
	// vector type to keep this example simple. A STL vector would work the
	// same for the implementation approach.

    float a = vec1[1]*vec2[2]-vec1[2]*vec2[1]; 
    float b = vec1[2]*vec2[0]-vec1[0]*vec2[2]; 
    float c = vec1[0]*vec2[1]-vec1[1]*vec2[0]; 
	Eigen::Vector3f result(3);
    result << a,b,c;
	return result;
}
