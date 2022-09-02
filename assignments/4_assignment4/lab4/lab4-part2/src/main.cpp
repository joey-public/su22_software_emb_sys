#include <iostream>
#include <string>
#include <cmath>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "img_proc.h"
#include "timer.h"

#define OPENCV 0
#define CPU 1
#define GPU 2

#define BLUR_SIZE 5

#define UNIFIED_MEM 

using namespace std;
using namespace cv;

int usage()
{
	cout << "Usage: ./lab4 <mode> <WIDTH> <HEIGHT>" <<endl;
	cout << "mode: 0 OpenCV" << endl;
	cout << "      1 CPU" << endl;
	cout << "      2 GPU" << endl;
	return 0;
}

int use_mode(int mode)
{
	string descr;
	switch(mode)
	{
		case OPENCV:
			descr = "OpenCV Functions";
			break;
		case CPU:
			descr = "CPU Implementations";
			break;
		case GPU:
			descr = "GPU Implementations";
			break;
		default:
			descr = "None";
			return usage();
	}	
	
	cout << "Using " << descr.c_str() <<endl;
	return 1;
}

int main(int argc, const char *argv[]) 
{

	int mode = 0;

	if(argc >= 2)
	{
		mode = atoi(argv[1]);	
	}
	
	if(use_mode(mode) == 0)
		return 0;

	VideoCapture cap("input.raw");

	int WIDTH  = 768;
	int HEIGHT = 768;
	int CHANNELS = 3;

	// 1 argument on command line: WIDTH = HEIGHT = arg
	if(argc >= 3)
	{
		WIDTH = atoi(argv[2]);
		HEIGHT = WIDTH;
	}
	// 2 arguments on command line: WIDTH = arg1, HEIGHT = arg2
	if(argc >= 4)
	{
		HEIGHT = atoi(argv[3]);
	}

	// Profiling framerate
	LinuxTimer timer;
	LinuxTimer fps_counter;
	double time_elapsed = 0;

    uint size_img = WIDTH*HEIGHT*sizeof(unsigned char);
    unsigned char *rgb_device, *gray_device, *invert_device, *blur_device;

#ifndef UNIFIED_MEM
    //TODO: Allocate memory on the GPU device.
    //TODO: Declare the host image result matrices
    printf("\nSeperate Memory\n");
	Mat rgb = Mat(HEIGHT, WIDTH, CV_8UC3);
	Mat gray = Mat(HEIGHT, WIDTH, CV_8U);
    Mat invert = Mat(HEIGHT, WIDTH, CV_8U);
    Mat blur = Mat(HEIGHT, WIDTH, CV_8U);
#else
    printf("\nUnified Memory\n");
    cudaMallocManaged(&rgb_device, size_img*CHANNELS);
    cudaMallocManaged(&gray_device, size_img);
    cudaMallocManaged(&invert_device, size_img);
    cudaMallocManaged(&blur_device, size_img);
    Mat rgb = Mat(HEIGHT, WIDTH, CV_8UC3, rgb_device);
    Mat gray = Mat(HEIGHT, WIDTH, CV_8U, gray_device);
    Mat invert = Mat(HEIGHT, WIDTH, CV_8U, invert_device);
    Mat blur = Mat(HEIGHT, WIDTH, CV_8U, blur_device);
#endif

	//Matrix for OpenCV inversion
	Mat ones = Mat::ones(HEIGHT, WIDTH, CV_8U)*255;

	Mat frame;	
	char key=0;
	int count = 0;

	while (key != 'q')
	{
		cap >> frame;
		if(frame.empty())
		{
			waitKey();
			break;
		}

		resize(frame, rgb, Size(WIDTH, HEIGHT));

		imshow("Original", rgb);

		timer.start();
		switch(mode)
		{
			case OPENCV:
#ifdef OPENCV4
				cvtColor(rgb, gray, COLOR_BGR2GRAY);
#else
				cvtColor(rgb, gray, CV_BGR2GRAY);
#endif
                invert = 255-gray;
                cv::blur(gray, blur, Size(BLUR_SIZE,BLUR_SIZE), Point(-1,-1));
				break;
			case CPU:
                img_rgb2gray_cpu(gray.ptr(), rgb.ptr(), WIDTH, HEIGHT, CHANNELS); 
                img_invert_cpu(invert.ptr<uchar>(), gray.ptr<uchar>(), WIDTH, HEIGHT);
                img_blur_cpu(blur.ptr<uchar>(), gray.ptr<uchar>(), WIDTH, HEIGHT, BLUR_SIZE);
				break;

			case GPU:
#ifndef UNIFIED_MEM
                cudaMalloc((void**)&rgb_device, size_img*CHANNELS);
                cudaMalloc((void**)&gray_device, size_img);
                cudaMalloc((void**)&invert_device, size_img);
                cudaMalloc((void**)&blur_device, size_img);
                cudaMemcpy(rgb_device, rgb.ptr<uchar>(), size_img*CHANNELS, cudaMemcpyHostToDevice);
                img_rgb2gray(gray_device, rgb_device, WIDTH, HEIGHT, CHANNELS);
                img_invert(invert_device, gray_device, WIDTH, HEIGHT);
                img_blur(blur_device, gray_device, WIDTH, HEIGHT, BLUR_SIZE);
                cudaMemcpy(gray.ptr<uchar>(), gray_device, size_img, cudaMemcpyDeviceToHost);
                cudaMemcpy(invert.ptr<uchar>(), invert_device, size_img, cudaMemcpyDeviceToHost);
                cudaMemcpy(blur.ptr<uchar>(), blur_device, size_img, cudaMemcpyDeviceToHost);
                cudaFree(rgb_device);
                cudaFree(gray_device);
                cudaFree(invert_device);
                cudaFree(blur_device);
#else
                img_rgb2gray(gray.ptr<uchar>(), rgb.ptr<uchar>(), WIDTH, HEIGHT, CHANNELS);
                img_invert(invert_device, gray_device, WIDTH, HEIGHT);
                img_blur(blur_device, gray_device, WIDTH, HEIGHT, BLUR_SIZE);
#endif
				break;
		}
		timer.stop();

		size_t time_rgb2gray = timer.getElapsed();
		
		count++;
		time_elapsed += (timer.getElapsed())/10000000000.0;

		if (count % 10 == 0)
		{
			cout << "Execution Time (s) = " << time_elapsed << endl;
			time_elapsed = 0;
		}

		imshow("Gray", gray);
		imshow("Invert", invert);
		imshow("Blur", blur);

		key = waitKey(1);
	}
}
