# Lab 3 

## FIR Filter 
###  1. Gprof Comparison For FIR Filter Jetson
| Opt Level| Jet-Naive | Jet-Opt   | Jet-Neon  |
| -------- | --------- | --------- | --------- |
| O0       | 0.05 ms   | 0.02 ms   | 0.04 ms   |
| O1       | 0.02 ms   | 0.00 ms   | 0.01 ms   |
| O2       | 0.02 ms   | 0.00 ms   | 0.01 ms   |
| O3       | 0.01 ms   | 0.01 ms   | 0.00 ms   |
| Ofast    | 0.01 ms   | 0.01 ms   | 0.00 ms   |

The FIR filter was running so fast that gprof started showing 0.00 for the loop unrolled at just the O1 oprimization. Still In general the speed increased as the optimization flags increased. 

## 2-3. Time Comparison For FIR Filter on PYNQ vs Jetson (o0)
| Time     | Pynq    | Pynq pg | Jetson    | Jetson pg |
| -------- | ------- | ------- | --------- | --------- |
| real     | 1.732 s | 1.729 s | 0.266 s   | 0.263 s   |
| usr      | 1.690 s | 1.708 s | 0.252 s   | 0.252 s   |
| sys      | 0.040 s | 0.020 s | 0.120 s   | 0.008 s   |

This test was run using the `-O0` optimization and using the `time` bash program to time the program execution. Compiling with and without the -pq flag made a slight difference in the execution time, but not a significant one. What made a bigger differnce was which cpu the program was run on. As seen in the above table the processor on the PYNQ board is quite a bit slower than the one on the Jetson TX2. 

# Assingment 3
## Sobel Filter 
### 1. Neon Description
While working on my neon implementation I focused on using neon intrinsics to do the sobel x and y kernel calculations. I used `floa32x4` neon registers for all of my math. I used a total of 5 registers to sotor input image data, x sobel kernal data, y sobel kernel data, x result data, and y result data. 
For each pixel in the input image I stared by sering out the result x and y registers. I also declared pointers for the image region and x and y kernels to be used in SIMD calculations. Next I loaded 4 floats into the input image register and the kernel x and y registers. I then filled the result x and y registes by adding the result of multipying the results of adding the multiplication results of the input image and kerel x and y registers.
9 total mulitplies and adds are needed so 4 wide registers take care of 8 out of the 9 calculations. The 9th calcuation if added and then the sqrt(x^2+y^2) result is stored in the output matrix. 
### 2-3. Sobel Filter Profiling 
The below block shows a summary of the gprof results for running `./hw3 5`. The code was compiled with `-O1` optimiation and `-pg` debugging, and run on the PYNQ Z2 board. I edited the main.cpp file so that passing the argument 5 runs a profile funciton that just calls all of the sobel functions. Then compiling with `-pg` running `./hw3` and then running `gprof -b hw3 gmon.out` give a good summary of how perfomant each sobel implementations was. Interestingly my naive solution was the most performant besides the opencv implementation. My loop unrolling was the least performant and the neon implementaion was in the middle. I think that the compiler was easily able to understand what I was doing in my naive implementaiton because it was mostly straightforward. Then the compiler must have done some black magic to optimize the naive code. With the loop unroll and neon code my approach involved creating a new 3x3 matrix for each pixel iteration. This matrix was needed to store the desired submatrix of the imput image in sequential row Major order memory. I was thinking that this would imporove perfomance becasue once the new 3x3 matrix was formed acessing memory during the sobel kernel calulations would be fast since the memory is all localized. I think that I actually ended up moveing and recasting too much data in my unrolling approach which overall lead to a slower funciton than the naive. If I was going to try again I think I would completely unroll the inner 9 multiply and add calculations instead of having an inner loop that iterates only twice. I had the inner loop that iterated by 4 twice because I was thinking ahead about the neon imlpemetaion using `float32x4_t` registers. I think the neon implementation was faster than the loop unroll becasue it was basically the exact same logic but there were 12 fewer multiply and add instructions per pixel since the SIMD neon regiseters took care of the 16 inner loop calculations in just 4 SIMD instuctions. Also the OpenCV sobel implementaion was much faster than any of my implementations. In fact it is too fast for the `gprof` to profile as it is just showing 0.00 s and 0% runtime for the 294x294 image testcase. 
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
## Extra Credit: Valgrind Cache Analysis 
The extra toolprofiling tool I decided to test was `valgrind`. SPecifically i was using the `cachegrind` tool to analyze the cache misses for the differtn sobel algorithms. To use cachegrind I comiled with debug `-g` and then ran the `valgrind --tool=cachegrind ./hw3 #`. Then I ran this for my Naive, loop unrolled and neon sobel filters. THe cack performance summaries for each are shown below. From https://valgrind.org/docs/manual/cg-manual.html you can see that I1 misses estimate instuction cache misses, D1 simulate data chache misses and LL simulates a Last Level Cache. The cache simualtion results show that the naive implementation had by far the smallest I1 chache miss rate, which is probably why it was the fastest to execute of the three. Whats interesting was that the neon implementation had more I1 misses than the loop unroll, but stil executed faster. 
### Naive Cache Performance
```
==2027== Process terminating with default action of signal 27 (SIGPROF)
==2027==    at 0x50145A2: syscall (syscall.S:38)
==2027==    by 0x81ED589: ??? (in /usr/lib/arm-linux-gnueabihf/libtbb.so.2)
==2027==    by 0x81ED5D5: ??? (in /usr/lib/arm-linux-gnueabihf/libtbb.so.2)
==2027==    by 0x59199D1: start_thread (pthread_create.c:477)
==2027==    by 0x5016C5B: ??? (clone.S:73)
==2027==
==2027== I   refs:      895,830,047
==2027== I1  misses:        337,619
==2027== LLi misses:         27,169
==2027== I1  miss rate:        0.04%
==2027== LLi miss rate:        0.00%
==2027==
==2027== D   refs:      434,890,858  (301,991,363 rd   + 132,899,495 wr)
==2027== D1  misses:     16,914,213  ( 16,282,498 rd   +     631,715 wr)
==2027== LLd misses:      3,198,501  (  2,843,615 rd   +     354,886 wr)
==2027== D1  miss rate:         3.9% (        5.4%     +         0.5%  )
==2027== LLd miss rate:         0.7% (        0.9%     +         0.3%  )
==2027==
==2027== LL refs:        17,251,832  ( 16,620,117 rd   +     631,715 wr)
==2027== LL misses:       3,225,670  (  2,870,784 rd   +     354,886 wr)
==2027== LL miss rate:          0.2% (        0.2%     +         0.3%  )
```
### Loop Unroll Cache Performance
```
==2075==
==2075== Process terminating with default action of signal 27 (SIGPROF)
==2075==    at 0x50145A2: syscall (syscall.S:38)
==2075==    by 0x81ED589: ??? (in /usr/lib/arm-linux-gnueabihf/libtbb.so.2)
==2075==    by 0x81ED5D5: ??? (in /usr/lib/arm-linux-gnueabihf/libtbb.so.2)
==2075==    by 0x59199D1: start_thread (pthread_create.c:477)
==2075==    by 0x5016C5B: ??? (clone.S:73)
==2075==
==2075== I   refs:      1,222,711,226
==2075== I1  misses:        9,826,516
==2075== LLi misses:           28,012
==2075== I1  miss rate:          0.80%
==2075== LLi miss rate:          0.00%
==2075==
==2075== D   refs:        642,063,514  (429,729,399 rd   + 212,334,115 wr)
==2075== D1  misses:       21,184,728  ( 20,331,512 rd   +     853,216 wr)
==2075== LLd misses:        3,197,420  (  2,843,113 rd   +     354,307 wr)
==2075== D1  miss rate:           3.3% (        4.7%     +         0.4%  )
==2075== LLd miss rate:           0.5% (        0.7%     +         0.2%  )
==2075==
==2075== LL refs:          31,011,244  ( 30,158,028 rd   +     853,216 wr)
==2075== LL misses:         3,225,432  (  2,871,125 rd   +     354,307 wr)
==2075== LL miss rate:            0.2% (        0.2%     +         0.2%  )
```
### Neon Cache Performance
```
==2100==
==2100== Process terminating with default action of signal 27 (SIGPROF)
==2100==    at 0x50145A2: syscall (syscall.S:38)
==2100==    by 0x81ED589: ??? (in /usr/lib/arm-linux-gnueabihf/libtbb.so.2)
==2100==    by 0x81ED5D5: ??? (in /usr/lib/arm-linux-gnueabihf/libtbb.so.2)
==2100==    by 0x59199D1: start_thread (pthread_create.c:477)
==2100==    by 0x5016C5B: ??? (clone.S:73)
==2100==
==2100== I   refs:      1,420,555,177
==2100== I1  misses:       14,348,981
==2100== LLi misses:           27,971
==2100== I1  miss rate:          1.01%
==2100== LLi miss rate:          0.00%
==2100==
==2100== D   refs:        785,885,798  (515,285,551 rd   + 270,600,247 wr)
==2100== D1  misses:       22,945,313  ( 21,756,466 rd   +   1,188,847 wr)
==2100== LLd misses:        3,196,819  (  2,842,578 rd   +     354,241 wr)
==2100== D1  miss rate:           2.9% (        4.2%     +         0.4%  )
==2100== LLd miss rate:           0.4% (        0.6%     +         0.1%  )
==2100==
==2100== LL refs:          37,294,294  ( 36,105,447 rd   +   1,188,847 wr)
==2100== LL misses:         3,224,790  (  2,870,549 rd   +     354,241 wr)
==2100== LL miss rate:            0.1% (        0.1%     +         0.1%  )
```

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
