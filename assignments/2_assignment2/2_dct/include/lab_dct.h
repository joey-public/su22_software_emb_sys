#ifndef LAB_DCT_H
#define LAB_DCT_H

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
cv::Mat lab_dct_naive(cv::Mat input);
cv::Mat lab_dct_opt(cv::Mat input);

#endif
