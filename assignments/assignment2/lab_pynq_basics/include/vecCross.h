// Include guard, one should be in all headers
#ifndef VECCROSS_H
#define VECCROSS_H

// Include the Eigen pacakge that provides vec classes
#include <Eigen/Core>

// Calculate the dot product between two 3 Dimensional vectors
//  @param     vec1 First (3D) vector
//  @param     vec2 Second (3D) vector
//  @return        Cross product of the vectors
Eigen::VectorXf cross_product_3d(Eigen::VectorXf vec1, Eigen::VectorXf vec2);
#endif // VECMUL_H
