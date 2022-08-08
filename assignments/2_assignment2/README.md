## WES 237B Assignment 2

## Part 1: Welcome to PYNQ
### Compile and Run Instructions
```
    cd 1_lab_pynq_basics
    make
    bin/vectest
```
### Description
For this part of the assignment I first installed the Eigen C++ library. I used the Eigen `vector` type and implemented custom dot and cross product functions, in the `vecDot.cxx` and `vecCross.cxx` files. In the `vecMultTest.cxx` file I tested the results of my custom functions to the built in dot and cross product functions within the Eigen library. The main function is in `vecMain.cxx` and it runs the tests to make sure my custom functions are working and then prints the results from an example dot and cross product. 
### Deliverables
*None*

## Part 2: Welcome to OpenCV
### Compile and Run Instructions
```
    cd 2_dct 
    make
    bin/dcttest arg
```
**Note arg should be `0` for naive DCT and `1` for separable DCT.**
### Description
In Part 2 of the assignment I compared the performance of a naive Discrete Cosine Transform(DCT) algorithm to a more optimized DCT using a separable algorithm. The DCT algorithms are in the `lab_dct.cxx` file. The main function is in `main.cxx`. The main function takes `image.tif` as a input, runs both DCT algorithms and compares the time it takes to run each algoImages.ipynb)
2. Results from running with `time`
```
```

## Part 3: Matrix Multiplication and Performance
### Compile and Run Instructions
```
    insmod kernal_module/CPUcntr.ko
    cd 3_matMult
    make
    bin/matMultTest
```
### Description
For the 3rd part of the assignment I implemented a naive matrix multiplication algorithm and compared it to the Eigen and OpenCV library matrix multiplication results. The `myMatMult.cxx` holds the implementation of my naive matrix multiplication algorithm and `main.cxx` contains the logic to perform matrix multiplication with all 3 methods and compare the results. Performance in this part of the lab was monitored using the PMU counter we created fin WES237A. This keeps track of how many clock cycle the cpu uses to do the each matrix multiplication.
### Deliverables
1. Performance Comparison Results
```
```


## Part 4: 2D DCTs
### Compile and Run Instructions
```
    cd 4_dct2d
    make
    bin/dct2d width height
```
**Note `width` and `height` specify the size to rescale the image to. For the code to work as expected `width` and `height` should be the same value and be divisible by 8. Passing no args will default to 64x64. Passing just the `width` will make `height=width`.**
### Description
Part 4 of the assignment used many building blocks from the first 3 parts of the lab to implement 4 different 2d DCT algorithms and compare their performance. The 4 algorithms that were implemented were a Naive 4 nested loop DCT, a 2D separable DCT, Matrix Multiplication DCT, and a clock Matrix Multiply DCT. A DCT coefficient matrix was created and all four algorithms use the matrix in their implementation. The Matrix Multiplication and Block Matrix Multiplication use the DCT matrix directly to get the DCT result. The naive and separable algorithms use the DCT matrix as a lookup table to reduce the loop calculations by replacing them with a coefficient lookup. All of the DCT algorithms are implemented in the `student_dct.cxx` file. The main function is in the `main.cxx` file and it starts by asking the user which algorithm to use and then runs the DCT using the OpenCV built in function and Custom implementation. The results from the selected custom DCT is compared to the results from the OpenCV DCT and the RMSE is reported. This process repeats on individual frames of the `inpit.raw` file.
### Deliverables
1. Performance Results 64x64
2. Performance Results 128x128
2. Performance Results 256x256
2. Performance Results 384x384
