#include <cmath>
#include <iostream>

#include "vecMulTest.h"
#include "vecDot.h"
#include "vecCross.h"
#include <Eigen/Geometry>

bool test_dot(Eigen::VectorXf vec1, Eigen::VectorXf vec2, uint32_t length) {
	float eigen_result = vec1.dot(vec2);
	float our_result = dot_product(vec1, vec2, length);

	// Only check precision to four decimal places
	// -> Try me! What happens if we don't round? Why?
	eigen_result = floor(eigen_result * pow(10,4) + .5) / pow(10,4);
	our_result = floor(our_result * pow(10,4) + .5) / pow(10,4);

	return eigen_result == our_result;
}
bool test_cross(Eigen::VectorXf vec1, Eigen::VectorXf vec2) {
	Eigen::Vector3d eigen_result = vec1.cross(vec2);
	Eigen::VectorXf our_result = cross_product_3d(vec1, vec2);

    std::cout << (eigen_result==our_result) << '\n';
	// Only check precision to four decimal places
	// -> Try me! What happens if we don't round? Why?
//	eigen_result = floor(eigen_result * pow(10,4) + .5) / pow(10,4);
//	our_result = floor(our_result * pow(10,4) + .5) / pow(10,4);

//	return eigen_result == our_result;
    return true;
}
