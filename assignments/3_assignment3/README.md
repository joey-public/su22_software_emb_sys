#Lab 3 

## FIR Filter 
###  1. Gprof Comparison For FIR Filter Jetson
| Opt Level| Pynq n  | Pynq o  | Pynq s  | Jetson n  | Jetson o  | Jetson s  |
| -------- | ------- | ------- | ------- | --------- | --------- | --------- |
| O0       | 0.00 ms | 0.00 ms | 0.00 ms | 0.05 ms   | 0.02 ms   | 0.04 ms   |
| O1       | 0.00 ms | 0.00 ms | 0.00 ms | 0.02 ms   | 0.00 ms   | 0.01 ms   |
| O2       | 0.00 ms | 0.00 ms | 0.00 ms | 0.02 ms   | 0.00 ms   | 0.01 ms   |
| O3       | 0.00 ms | 0.00 ms | 0.00 ms | 0.01 ms   | 0.01 ms   | 0.00 ms   |
| Ofast    | 0.00 ms | 0.00 ms | 0.00 ms | 0.01 ms   | 0.01 ms   | 0.00 ms   |

## 2 & 3. Time Comparison For FIR Filter on PYNQ vs Jetson (o0)
| Time     | Pynq    | Pynq pg | Jetson    | Jetson pg |
| -------- | ------- | ------- | --------- | --------- |
| real     | 1.732 s | 1.729 s | 0.266 s   | 0.263 s   |
| usr      | 1.690 s | 1.708 s | 0.252 s   | 0.252 s   |
| sys      | 0.040 s | 0.020 s | 0.120 s   | 0.008 s   |

Compiling with and without the -pq flag made a slight difference in the execution time, but not a significant one. What made a bigger differnce was which cpu the program was run on. As seen in the above table the processor on the PYNQ board is quite a bit slower than the one on the Jetson TX2. 

# Assingment 3

## Sobel Filter 

### 1. Neon Description
While working on my neon implementation I focused on using neon intrinsics to do the sobel x and y kernel calculations. I used `floa32x4` neon registers for all of my math. I used a total of 5 registers to sotor input image data, x sobel kernal data, y sobel kernel data, x result data, and y result data. 
For each pixel in the input image I stared by sering out the result x and y registers. I also declared pointers for the image region and x and y kernels to be used in SIMD calculations. Next I loaded 4 floats into the input image register and the kernel x and y registers. I then filled the result x and y registes by adding the result of multipying the results of adding the multiplication results of the input image and kerel x and y registers.
9 total mulitplies and adds are needed so 4 wide registers take care of 8 out of the 9 calculations. The 9th calcuation if added and then the sqrt(x^2+y^2) result is stored in the output matrix. 
### 2 and 3. Sobel Filter Profiling 
The below block shows a summary of the gprof results for running ./hw3 5. The code was compiled with `-O1` optimiation and `-pg` debugging, and run on the PYNQ Z2 board. I edited the main.cpp file so that passing the argument 5 runs a profile funciton that just calls all of the sobel functions. Then compiling with `-pg` running `./hw3` and then running `gprof -b hw3 gmon.out` give a good summary of how perfomant each sobel implementations was. Interestingly my naive solution was the most performant besides the opencv implementation. My loop unrolling was the least performant and the neon implementaion was in the middle. 
I think that the compiler was easily able to understand what I was doing in my naive implementaiton because it was mostly straightforward. Then the compile must have done some black magic to optimize the naive code. For the loop unroll and neon my approach involved creating a new 3x3 matrix for each pixel iteration. This matrix was needed to store the desired submatrix of the imput image in sequential row Major order memory. I was thinking that this would imporove perfomance becasue once the new 3x3 matrix was formed acessing memory during the sobel kernel calulations would be fast since the memory is all localized. I am not sure, but I think that I actually ended up moved and recasting too much data in my unrolling approach which overall lead to a slower funciton than the naive. 
If I was going to try again I think I would completely unroll the inner 9 multiply and add calculations instead of having an inner loop that iterates only twice. I had the inner loop that iterated by 4 twice because I was thinking ahead about the neon imlpemetaion using `float32x4_t` registers. I think the neon implementation was faster than the loop unroll becasue it was basically the exact same logic but there were 12 fewer multiply and add instructions per pixel since the SIMD neon regiseters took care of the 16 inner loop calculations in just 4 SIMD instuctions.  
ALso the OpenCV sobel implementaion was much faster than any of my implementations. In fact it is too fast for the `gprof` to profile as it is just showing 0.00 s and 0% runtime for the 294x294 image testcase. 
```
Flat profile:

Each sample counts as 0.01 seconds.
  %   cumulative   self              self     total
 time   seconds   seconds    calls  ms/call  ms/call  name
 53.12      0.17     0.17        1   170.00   170.00  sobel_unroll(cv::Mat const&, cv::Mat&)
 31.25      0.27     0.10        1   100.00   100.00  sobel_neon(cv::Mat const&, cv::Mat&)
 15.62      0.32     0.05        1    50.00    50.00  sobel(cv::Mat const&, cv::Mat&)
  0.00      0.32     0.00        4     0.00     0.00  cv::Mat::~Mat()
  0.00      0.32     0.00        1     0.00     0.00  _GLOBAL__sub_I__Z5usagev
  0.00      0.32     0.00        1     0.00     0.00  _GLOBAL__sub_I_sobel_kernel_x
  0.00      0.32     0.00        1     0.00     0.00  sobel_opencv(cv::Mat const&, cv::Mat&)
  0.00      0.32     0.00        1     0.00   320.00  profile(cv::Mat)
  0.00      0.32     0.00        1     0.00     0.00  cv::MatExpr::~MatExpr()
```
## Extra Credit: Gcov Profiling

## CUDA Intro
1. What does the global flag mean?
    `__global__` is a keyword that tell the nvcc comiler that a function should run on the device (GPU).
2. Explain the following line of code in terms of threads, blocks, and grids: `foo<<4,32>>(out, in1, in2)`
    This line creates a new kernel with the `<<<>>>` syntax. The `4` indicates that this kernel will have 4 blocks. The `32` indicated that each block contains 32 threads. So overall this kernel will perform `4*32=128` parallel calculations when it is executed.  


# Apendix: full gprof of sobel filter
```
Flat profile:

Each sample counts as 0.01 seconds.
  %   cumulative   self              self     total
 time   seconds   seconds    calls  ms/call  ms/call  name
 53.12      0.17     0.17        1   170.00   170.00  sobel_unroll(cv::Mat const&, cv::Mat&)
 31.25      0.27     0.10        1   100.00   100.00  sobel_neon(cv::Mat const&, cv::Mat&)
 15.62      0.32     0.05        1    50.00    50.00  sobel(cv::Mat const&, cv::Mat&)
  0.00      0.32     0.00        4     0.00     0.00  cv::Mat::~Mat()
  0.00      0.32     0.00        1     0.00     0.00  _GLOBAL__sub_I__Z5usagev
  0.00      0.32     0.00        1     0.00     0.00  _GLOBAL__sub_I_sobel_kernel_x
  0.00      0.32     0.00        1     0.00     0.00  sobel_opencv(cv::Mat const&, cv::Mat&)
  0.00      0.32     0.00        1     0.00   320.00  profile(cv::Mat)
  0.00      0.32     0.00        1     0.00     0.00  cv::MatExpr::~MatExpr()


                        Call graph


granularity: each sample hit covers 4 byte(s) for 3.12% of 0.32 seconds

index % time    self  children    called     name
                                                 <spontaneous>
[1]    100.0    0.00    0.32                 main [1]
                0.00    0.32       1/1           profile(cv::Mat) [2]
                0.00    0.00       4/4           cv::Mat::~Mat() [13]
                0.00    0.00       1/1           cv::MatExpr::~MatExpr() [17]
-----------------------------------------------
                0.00    0.32       1/1           main [1]
[2]    100.0    0.00    0.32       1         profile(cv::Mat) [2]
                0.17    0.00       1/1           sobel_unroll(cv::Mat const&, cv::Mat&) [3]
                0.10    0.00       1/1           sobel_neon(cv::Mat const&, cv::Mat&) [4]
                0.05    0.00       1/1           sobel(cv::Mat const&, cv::Mat&) [5]
                0.00    0.00       1/1           sobel_opencv(cv::Mat const&, cv::Mat&) [16]
-----------------------------------------------
                0.17    0.00       1/1           profile(cv::Mat) [2]
[3]     53.1    0.17    0.00       1         sobel_unroll(cv::Mat const&, cv::Mat&) [3]
-----------------------------------------------
                0.10    0.00       1/1           profile(cv::Mat) [2]
[4]     31.2    0.10    0.00       1         sobel_neon(cv::Mat const&, cv::Mat&) [4]
-----------------------------------------------
                0.05    0.00       1/1           profile(cv::Mat) [2]
[5]     15.6    0.05    0.00       1         sobel(cv::Mat const&, cv::Mat&) [5]
-----------------------------------------------
                0.00    0.00       4/4           main [1]
[13]     0.0    0.00    0.00       4         cv::Mat::~Mat() [13]
-----------------------------------------------
                0.00    0.00       1/1           __libc_csu_init [23]
[14]     0.0    0.00    0.00       1         _GLOBAL__sub_I__Z5usagev [14]
-----------------------------------------------
                0.00    0.00       1/1           __libc_csu_init [23]
[15]     0.0    0.00    0.00       1         _GLOBAL__sub_I_sobel_kernel_x [15]
-----------------------------------------------
                0.00    0.00       1/1           profile(cv::Mat) [2]
[16]     0.0    0.00    0.00       1         sobel_opencv(cv::Mat const&, cv::Mat&) [16]
-----------------------------------------------
                0.00    0.00       1/1           main [1]
[17]     0.0    0.00    0.00       1         cv::MatExpr::~MatExpr() [17]
-----------------------------------------------


Index by function name

  [14] _GLOBAL__sub_I__Z5usagev [16] sobel_opencv(cv::Mat const&, cv::Mat&) [2] profile(cv::Mat)
  [15] _GLOBAL__sub_I_sobel_kernel_x [3] sobel_unroll(cv::Mat const&, cv::Mat&) [13] cv::Mat::~Mat()
   [4] sobel_neon(cv::Mat const&, cv::Mat&) [5] sobel(cv::Mat const&, cv::Mat&) [17] cv::MatExpr::~MatExpr()
```
