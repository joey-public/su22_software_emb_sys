#include "main.h"

// ----------------------------------------------
// Run a FIR filter on the given input data
// ----------------------------------------------
void fir(float *coeffs, float *input, float *output, int length, int filterLength)
// ----------------------------------------------
{
    std::cout << "Naive FIR\n";
    for(int i=0; i<length-filterLength; i++){//for size of signal
        float result = 0.0;
        for(int j=0; j<filterLength; j++){//for each filter coef 
            result += input[i+j] * coeffs[j]; 
        }
        output[i] = result;
    }
}

// ----------------------------------------------
// Run a FIR filter on the given input data using Loop Unrolling
// ----------------------------------------------
void fir_opt(float *coeffs, float *input, float *output, int length, int filterLength)
// ----------------------------------------------
{
    std::cout << "Loop Unroll FIR\n";
    for(int i=0; i<length-filterLength; i++){//for size of signal
        float result[4] = {0.0};
        for(int j=0; j<filterLength; j+=4){//for each filter coef 
            result[0] += input[i+j+0] * coeffs[j+0];
            result[1] += input[i+j+1] * coeffs[j+1];
            result[2] += input[i+j+2] * coeffs[j+2];
            result[3] += input[i+j+3] * coeffs[j+3];
        }
        output[i] = result[0]+result[1]+result[2]+result[3];
    }
}

// ----------------------------------------------
// Run a FIR filter on the given input data using NEON
// ----------------------------------------------
void fir_neon(float *coeffs, float *input, float *output, int length, int filterLength)
// ----------------------------------------------
{
    float32x4_t ac;
    float32x4_t in;
    float32x4_t co;
    for(int i=0; i<length-filterLength; i++){
        ac = vmovq_n_f32(0);
        for(int j=0; j<filterLength; j+=4){
            in = vld1q_f32(&input[i+j]); 
            co = vld1q_f32(&coeffs[j]); 
            ac = vaddq_f32(ac, vmulq_f32(in,co));
        }
    output[i] = ac[0] + ac[1] + ac[2] + ac[3];
    }
}


// ----------------------------------------------
// Create filter coefficients
// ----------------------------------------------
void designLPF(float* coeffs, int filterLength, float Fs, float Fx)
// ----------------------------------------------
{
	float lambda = M_PI * Fx / (Fs/2);

	for(int n = 0; n < filterLength; n++)
	{
		float mm = n - (filterLength - 1.0) / 2.0;
		if( mm == 0.0 ) coeffs[n] = lambda / M_PI;
		else coeffs[n] = sin( mm * lambda ) / (mm * M_PI);
	}
}
