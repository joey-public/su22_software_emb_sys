#include "main.h"

#define ONE_OVER_ROOT_TWO 0.70710678118;
using namespace cv;

Mat LUT_w;
Mat LUT_h;

// Helper function
float sf(int in){
	if (in == 0)
		return ONE_OVER_ROOT_TWO; // = 1 / sqrt(2)
	return 1.;
}

Mat genDCTMat(int N){
    Mat out = Mat(N,N,CV_32FC1);
    float* optr = out.ptr<float>();
    for(int r=0; r<N; r++){
        for(int c=0; c<N; c++){
            if(r==0){
                out.at<float>(r,c) = ONE_OVER_ROOT_TWO;
            }
            else{
                out.at<float>(r,c) = cos( (M_PI*(r)*(2*(c+1)-1))/(2*N) );
            }
        }
    }
    return out*(sqrt(2.0f/N));
}

// Initialize LUT
void initDCT(int WIDTH, int HEIGHT){
    LUT_w = genDCTMat(WIDTH);
    LUT_h = genDCTMat(HEIGHT);
}

Mat student_dct_lut(Mat input){
    std::cout << "Naive Algorithm\n";
	const int HEIGHT = input.rows;
	const int WIDTH  = input.cols;
	float scale = 2./sqrt(HEIGHT*WIDTH);
	Mat result = Mat(HEIGHT, WIDTH, CV_32FC1);
	float* result_ptr = result.ptr<float>();
	float* input_ptr  = input.ptr<float>();
	for(int x = 0; x < HEIGHT; x++){
		for(int y = 0; y < WIDTH; y++){
			float value = 0.f;
			for(int i = 0; i < HEIGHT; i++){
				for(int j = 0; j < WIDTH; j++){
					value += input_ptr[i * WIDTH + j]
                        * LUT_h.at<float>(x,i)
                        * LUT_w.at<float>(y,j);
						// TODO
						// --- Replace cos calculation by LUT ---
				}
			}
			result_ptr[x * WIDTH + y] = value;
		}
	}
	return result;
}

Mat lab_dct_opt(cv::Mat input){
    std::cout << "Seperable Algorithm\n";
	const int HEIGHT = input.rows;
	const int WIDTH = input.cols;
	float scale = 2./sqrt(HEIGHT*WIDTH);
	cv::Mat result = cv::Mat(HEIGHT, WIDTH, CV_32FC1);
	cv::Mat result_row = cv::Mat(HEIGHT, WIDTH, CV_32FC1);
	float* result_ptr = result.ptr<float>();
	float* input_ptr = input.ptr<float>();
	float value;
	for(int k=0; k<HEIGHT; k++) {
		for(int i=0; i<WIDTH; i++) {
			value = 0.0;
			for(int j=0; j<WIDTH; j++) {
				value += input.at<float>(k, j) * cos(M_PI/((float)WIDTH)*(j+1./2.)*(float)i);
			}
			result_row.at<float>(k,i) = value * sf(i);
		}
	}
	for(int k=0; k<WIDTH; k++) {
		for(int i=0; i<HEIGHT; i++) {
			value = 0.0;
			for (int j=0; j<HEIGHT; j++) {
				value += result_row.at<float>(j,k) * cos(M_PI/((float)HEIGHT)*(j+1./2.)*(float)i);
			}
			result.at<float>(i, k) = value*sf(i)*scale;
		}
	}
	return result;
}

Mat student_dct_mm(Mat input){
    std::cout << "Matrix Mult\n";
    assert(input.rows==input.cols);
    return LUT_w*input*LUT_w.t();
}

Mat blockMatMult(int rows, int cols, Mat mat1, Mat mat2)
{ 
    Mat matOut = Mat(mat1.rows, mat1.cols, CV_32FC1);
    Mat result = Mat(8, 8, CV_32FC1);
    matOut = 0.0; result =0.0;
    int r=0; int c=0;
    Rect region, region2, region3;
    for(int e=0; e<rows*cols; e++){
        if(e%rows==0 && e!=0){
            r+=1;
            c=0;
        }
        region = Rect(c*8,r*8,8,8);
        result = 0.0;
        for(int i=0; i< rows; i++){
            region2 = Rect(8*i,8*r,8,8);
            region3 = Rect(8*c,8*i,8,8);
            result += mat1(region2)*mat2(region3);
        }
        result.copyTo(matOut(region));
        c+=1;
    }
    return matOut;
}

Mat student_dct_bmm(Mat input){
    std::cout << "Block Matrix Mult\n";
    assert(input.rows==input.cols);
    assert(input.rows % 8 == 0);
    int x = input.cols / 8;
    int y = input.rows / 8;
    Mat A = blockMatMult(x, y, LUT_w, input);
    Mat B = blockMatMult(x, y, A, LUT_w.t());
    return B; 
}

Mat student_dct(Mat input, int mode){
    if(mode == 0){
        return student_dct_lut(input);
    }
    else if(mode==1){
        return lab_dct_opt(input);
    }
    else if(mode==2){
        return student_dct_mm(input);
    }
    else if(mode==3){
        return student_dct_bmm(input); 
    }
    else{
        std::cout << "invalid mode\n";
        return student_dct_bmm(input); 
    }
}
