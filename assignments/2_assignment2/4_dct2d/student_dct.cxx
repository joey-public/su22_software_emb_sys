#include "main.h"

using namespace cv;
Mat LUT_w;
Mat LUT_h;
// Helper function
float sf(int in){
	if (in == 0)
		return 0.70710678118; // = 1 / sqrt(2)
	return 1.;
}
// Initialize LUT
void initDCT(int WIDTH, int HEIGHT)
{ 
	float scale = 2./sqrt(HEIGHT*WIDTH);
    LUT_w = Mat(1,WIDTH,CV_32F);
    LUT_h = Mat(1,HEIGHT,CV_32F);
    for(int i = 0; i < WIDTH; i++){
        LUT_w.at<float>(1,i) = scale * sf(i) * cos(M_PI/((float)WIDTH)*(i+1./2.));
    }
    for(int i = 0; i < HEIGHT; i++){
        LUT_h.at<float>(1,i) = scale * sf(i) * cos(M_PI/((float)HEIGHT)*(i+1./2.));
    }
}

// Baseline: O(N^4)
Mat student_dct_naive(Mat input)
{
	const int HEIGHT = input.rows;
	const int WIDTH = input.cols;
	float scale = 2./sqrt(HEIGHT*WIDTH);
	Mat result = Mat(HEIGHT, WIDTH, CV_32FC1);
	float* result_ptr = result.ptr<float>();
	float* input_ptr  = input.ptr<float>();
	for(int x = 0; x < HEIGHT; x++)
	{
		for(int y = 0; y < WIDTH; y++)
		{
			float value = 0.f;

			for(int i = 0; i < HEIGHT; i++)
			{
				for(int j = 0; j < WIDTH; j++)
				{
					value += input_ptr[i * WIDTH + j]
						* cos(M_PI/((float)HEIGHT)*(i+1./2.)*(float)x)
						* cos(M_PI/((float)WIDTH)*(j+1./2.)*(float)y);
				}
			}
			value = scale * sf(x) * sf(y) * value;
			result_ptr[x * WIDTH + y] = value;
		}
	}
	return result;
}

Mat student_dct_lut(Mat input)
{
	const int HEIGHT = input.rows;
	const int WIDTH  = input.cols;
	Mat result = Mat(HEIGHT, WIDTH, CV_32FC1);
	float* result_ptr = result.ptr<float>();
	float* input_ptr  = input.ptr<float>();
    float* LUT_h_ptr = LUT_h.ptr<float>();
    float* LUT_w_ptr = LUT_w.ptr<float>();
	for(int x = 0; x < HEIGHT; x++)
	{
		for(int y = 0; y < WIDTH; y++)
		{
			float value = 0.f;
			for(int i = 0; i < HEIGHT; i++)
			{
				for(int j = 0; j < WIDTH; j++)
				{
					value += input_ptr[i * WIDTH + j]
						* (*(LUT_h_ptr+x)) 
						* (*(LUT_w_ptr+y)); 
				}
			}
			result_ptr[x * WIDTH + y] = value;
		}
	}
	return result;
}


Mat student_dct(Mat input, int mode)
{
    if(mode == 0){
        std::cout << "Naive\n";
        return student_dct_naive(input);
    }
    else{
        std::cout << "LUT\n";
        return student_dct_lut(input);
    }
}


// *****************
//   Hint
// *****************
//
// DCT as matrix multiplication

/*
Mat student_dct(Mat input)
{
	// -- Works only for WIDTH == HEIGHT
	assert(input.rows == input.cols);
	
	// -- Matrix multiply with OpenCV
	Mat output = LUT_w * input * LUT_w.t();

	// TODO
	// Replace the line above by your own matrix multiplication code
	// You can use a temp matrix to store the intermediate result

	return output;
}
*/





