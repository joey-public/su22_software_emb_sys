# WES 237B Lab/Assignment 4

## Lab Part 1
### Compile and run instructions
```
```
### Deliverables
1.Explain in plain English what example 1 does.
- Example 1 sets up a basic cuda kernel. This kernel runs on the GPU(device). The kernel just acesses and prints values that are stored in an array a in the GPUs main memory. Each thread acesses one value of a, and the thread calulates which index to acess absed on its blockId, ThreadId and the blockDim.   
1.Explain the difference between example 1 and example 2. What is significant about it?
-Example 2 configues the GPU blocks and threads differnlly then Example 1. Example one used blocks and 2 threads per block to perform 4 parallel operations. Example 2 uses 1 block but 4 threads per block. Also example 2 stores its threads as a 2D (2x2) structure. As a result, Example 2 still has 4 total threads just like example 1, but the way the calculation for the index of a has changed. This is significant becasue it shows the importance of how the programmer decides to configure the device memory, Depending on the application how you layout the device memory can make for more or less efficeint solutions. 

2.Provide a copy of your myKernel() implementation from your matrix multiplication.
- Code is here: ./lab4/lab4-part1

3.Explain the difference between the two lw implementations.
- The first implementation uses manual memory management. Pinters for the device memory need so be declared and then CudaMalloc() is called to allocate GPU memory for the arrays. The programmer must manually copy data to and from the device using the cudaMemcpy() function and also free the device memory with CudaFree() when it5 is no longer needed. The second implementaiton uses managed memory, which automatically handles the memory management for you. You just call the cudaMallocManaged() function for each array you want on the device and then memory management is taken care of under the hood. This allows for less lines of code in your script since you can skip the cudaMemcpy() lines, but it also gives you slighlty less control over the device and host memory. It also abstracts the fact that the Jetson actually has spererate cpu and gpu memory.


## Lab Part 2
### Compile and run instructions
```
```
### Deliverables
1.Submit all of your final code for Part 2.
- All code is here: ./lab4/lab4-part2
2.For each algorithm (greyscale, inversion, blur), which implementation has the best performance? Make a quantitative case.
- OpenCV converts frames from rgb to grayscale in about 0.001 seconds on a 1024x1024 image. The CPU algorithm took about 0.03 seconds. The GPU algorithm without unified memory took about 0.006s. So the OpenCV fuinction was the fastest at converting the rgb image to grayscale, followed by the GPU and then finally the the CPU. 

- inversion
- OpenCV performs grayscale conversion + inversion in about 0.002 seconds. The CPU algorithm took about 0.03 seconds. The GPU algorithm without unified memory took about 0.008s. Subtracting out the time for just grayscale we get that openCV completed the inversion in 0.001 seconds, and the GPU finished in 0.002 seconds. Somehow the CPU time to convert to grayscale then invert was almost identicval to the CPU time to jsut convert to grayscale. Again openCV is the fastest, but here the GPU is not too far behind. The CPU was much faster at inverstion than grayscale conversion, probably because the input is a grayscale image which has a third as much data as the rgb image.   

- OpenCV performs grayscale conversion + blur in about 0.0035 seconds. The CPU algorithm took about 0.110 seconds. The GPU algorithm without unified memory took about 0.0132 seconds. Subtracting out the time for just grayscale we get that openCV completed the blur in 0.034 seconds,the CPu took around 0.080 sconds,  and the GPU finished in 0.0072 seconds. Again openCV is the fastest.


## Assignment Part 1: Sobel Filter
### Compile and run instructions
```
```
### Deliverables
1. Report approximate execution times for OpenCV Sobel, CPU Sobel,  and GPU Sobel for different image sizes.
512x512
1024x1024
4096x4096

## Assignemnt Part 2: Block Matrix Multiply
### Compile and run instructions
```
```
### Deliverables
1. Final Code
1. Results for various M and N values
1. optimal block/grid layout for 512x512 matrcies
