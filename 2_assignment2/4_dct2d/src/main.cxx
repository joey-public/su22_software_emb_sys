#include "main.h"
#include "timer.h"

#define FRAME_NUMBER 1 //set to 0 or -1 to run while loop

using namespace std;
using namespace cv;

int main(int argc, const char * argv[])
{
	unsigned int c_start;
	unsigned int opencv_c, student_c;
    cout << "-------\n";
	VideoCapture cap("input.raw");
	Mat frame, gray, dct_org, dct_student, diff_img;
	char key=0;
	float mse;
	int fps_cnt = 0;
    int MODE = 3;
	int WIDTH  = 64;
	int HEIGHT = 64;
	if(argc >= 2)
	{
        MODE = atoi(argv[1]);
	}
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
	initDCT(WIDTH, HEIGHT);
	//initDCT(8, 8);
	float avg_perf = 0.f;
	int count = 0;
#if FRAME_NUMBER <= 0
	while(key != 'q')
#else
    for(int f = 0; f < FRAME_NUMBER; f++)
#endif
	{
		cap >> frame;
		if(frame.empty()){ break; }
//imwrite("images/frame.tif",frame);
		cvtColor(frame, gray, COLOR_BGR2GRAY);
//imwrite("images/frame_gray.tif",gray);
		resize(gray, gray, Size(WIDTH, HEIGHT));
//imwrite("images/dct_input.tif", gray);
		gray.convertTo(gray, CV_32FC1);
		// OpenCV DCT
		dct(gray, dct_org);
		// Your DCT
		LinuxTimer t;
		dct_student = student_dct(gray,MODE);
		t.stop();
		float myTimer = t.getElapsed();
		gray.convertTo(gray, CV_8UC1);
        //dct_student.convertTo(dct_student, CV_8UC1);
        //dct_org.convertTo(dct_org, CV_8UC1);
		absdiff(dct_org, dct_student, diff_img); 
		/* calculating RMSE */
		diff_img = diff_img.mul(diff_img);
		Scalar se = sum(diff_img);
		mse = se[0]/((float)HEIGHT*WIDTH);
		count++;
		cout <<  "Execute time: "
			<< (double)myTimer/1000000000.0 << endl;
		printf("RMSE: %.4f\n", sqrt(mse));
		Mat inverse;
		idct(dct_student, inverse);
		inverse.convertTo(inverse, CV_8U);

//    dct_org.convertTo(dct_org, CV_8UC1);
//    imwrite("images/dct_org.tif",dct_org);
//    dct_org.convertTo(dct_student, CV_8UC1);
//    imwrite("images/dct_student.tif",dct_student);
//    imwrite("images/myInv.tif", inverse);
        
#ifndef __arm__
		imshow("Original", gray);
		imshow("IDCT Output", inverse);
		moveWindow("IDCT Output", 500, 0);
		key = waitKey(10);
#endif
	}
	return 0;
}



