#include <iostream>
#include <stdio.h>
#include <math.h>
#include <chrono>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "lab_dct.h"

using namespace std;
using namespace cv;

int main(int argc, const char * argv[]) {
	// Read the mode 
	int mode = stoi(argv[1]);

	Mat image;

	//Read in the image and store it in a matrix
	image = imread("images/image.tif");

	//Convert the image to grayscale
	Mat gray;
	cvtColor(image, gray, COLOR_BGR2GRAY);

	//Display the image (cannot be run on the PYNQ Board)
#ifndef __arm__
	imshow("Original Color", image);
#else
	imwrite("images/original_color.tif", gray);
#endif

	cvtColor(image, gray, COLOR_BGR2GRAY);
	resize(gray, gray, Size(64, 64));
	gray.convertTo(gray, CV_32FC1, 1.0/255.0);


	//OpenCV DCT function call
	if (mode == 1){
		Mat dct_cv;
        auto start = chrono::high_resolution_clock::now();
		dct(gray, dct_cv);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << "OpenCv DCT: " << duration.count()/1000.0 << "ms" << endl;
#ifndef __arm__
		imshow("DCT", dct_cv);
#else
		dct_cv.convertTo(dct_cv, CV_8UC1, 255.0);
		bool wrote = imwrite("images/opencv_dct.tif", dct_cv);
		if (wrote){
			cout << "Wrote opencv_dct.tif" << endl;
		}
#endif
	}

	// Naive DCT
	else if (mode == 2){
		Mat dct_lab;
        auto start = chrono::high_resolution_clock::now();
		dct_lab = lab_dct_naive(gray);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << "LabNaive DCT: " << duration.count()/1000.0 << " ms" << endl;
#ifndef __arm__
		imshow("DCT Lab Naive", dct_lab);
#else
		dct_lab.convertTo(dct_lab, CV_8UC1, 255.0);
		bool wrote = imwrite("images/dct_lab_naive.tif", dct_lab);
		if (wrote){
			cout << "Wrote dct_lab_naive.tif" << endl;
		}
#endif
	}

	// Optimized DCT
	else if (mode == 3){
		Mat dct_lab_opt;
        auto start = chrono::high_resolution_clock::now();
		dct_lab_opt = lab_dct_opt(gray);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << "LabOptimized DCT: " << duration.count()/1000.0 << " ms" << endl;
#ifndef __arm__
		imshow("DCT Lab Opt", dct_lab_opt);
#else
		dct_lab_opt.convertTo(dct_lab_opt, CV_8UC1, 255.0);
		bool wrote = imwrite("images/dct_lab_opt.tif", dct_lab_opt);
		if (wrote){
			cout << "Wrote dct_lab_opt.tif" << endl;
		}
#endif
	}

	else {
		cout << "Acceptable modes are 1, 2, 3: OpenCV, Naive, Optimized" <<endl;
	}

#ifndef __arm__
	//Wait for a key enter
	waitKey(0);
#endif
}
