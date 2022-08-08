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
1. ![Images](2_dct/displayImages.ipynb)
2. Results from running with `time`
    ```
        __OPENCV__
        OpenCv DCT: 0.782ms
        Wrote opencv_dct.tif
        
        real    0m0.707s
        user    0m0.621s
        sys     0m0.092s
        
        __NAIVE__
        LabNaive DCT: 5494.07 ms
        Wrote dct_lab_naive.tif
        
        real    0m6.203s
        user    0m6.050s
        sys     0m0.150s
        
        __SEPERABLE__
        LabOptimized DCT: 164.582 ms
        Wrote dct_lab_opt.tif
        
        real    0m0.874s
        user    0m0.753s
        sys     0m0.128s
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
        Naive Results:
        ----------
        Clock Cycles: 25
        22 , 13 , 30.9 ,
        49 , 31 , 70.8 ,
        76 , 49 , 110.7 ,
        -----
        
        OpenCv Results:
        ----------
        Clock Cycles: 2317
        [22, 13, 30.900002;
         49, 31, 70.800003;
         76, 49, 110.7]
        
        Eigen Results:
        ----------
        Clock Cycles: 40
           22    13  30.9
           49    31  70.8
           76    49 110.7
    ```


## Part 4: 2D DCTs
### Compile and Run Instructions
```
    cd 4_dct2d
    make
    bin/dct2d mode width height
```
**Note `mode` selects the algorithm should be 0-3. 0=naive, 1=seperable, 2=matrix multiply, 3=block matrix multiply. `width` and `height` specify the size to rescale the image to. For the code to work as expected `width` and `height` should be the same value and be divisible by 8. Passing no args will default to 64x64. Passing just the `width` will make `height=width`.**

There are also 4 shell scripts that can be run to test the results reported in the deliverables.
```
    cd 4_dct2d
    make
    source shell_scripts/batch_run64x64
```

### Description
Part 4 of the assignment used many building blocks from the first 3 parts of the lab to implement 4 different 2d DCT algorithms and compare their performance. The 4 algorithms that were implemented were a Naive 4 nested loop DCT, a 2D separable DCT, Matrix Multiplication DCT, and a clock Matrix Multiply DCT. A DCT coefficient matrix was created and all four algorithms use the matrix in their implementation. The Matrix Multiplication and Block Matrix Multiplication use the DCT matrix directly to get the DCT result. The naive and separable algorithms use the DCT matrix as a lookup table to reduce the loop calculations by replacing them with a coefficient lookup. All of the DCT algorithms are implemented in the `student_dct.cxx` file. The main function is in the `main.cxx` file and it runs the DCT using the OpenCV built in function and the custom implementation. The results from the selected custom DCT is compared to the results from the OpenCV DCT and the RMSE is reported. This process repeats on individual frames of the `inpit.raw` file.
### Deliverables
1. Performance Results 64x64
    ```
        -------
        Naive Algorithm
        Execute time: 0.211518
        RMSE: 0.0001
        
        real    0m1.858s
        user    0m1.676s
        sys     0m0.198s

        -------
        Seperable Algorithm
        Execute time: 0.163677
        RMSE: 0.0000
        
        real    0m1.814s
        user    0m1.641s
        sys     0m0.191s

        -------
        Matrix Mult
        Execute time: 0.00491731
        RMSE: 0.0000
        real    0m1.655s
        user    0m1.474s
        sys     0m0.200s

        -------
        Block Matrix Mult
        Execute time: 0.0201114
        RMSE: 0.0000
        real    0m1.664s
        user    0m1.462s
        sys     0m0.221s
    ```
2. Performance Results 128x128
    ```
        Batch Run for 128x128
        -------
        Naive Algorithm
        Execute time: 0.291521
        RMSE: 0.0002
        
        real    0m6.247s
        user    0m6.063s
        sys     0m0.201s
        
        -------
        Seperable Algorithm
        Execute time: 1.3934
        RMSE: 0.0000
        
        real    0m3.059s
        user    0m2.865s
        sys     0m0.211s
        
        -------
        Matrix Mult
        Execute time: 0.0611641
        RMSE: 0.0000
        
        real    0m1.732s
        user    0m1.509s
        sys     0m0.240s
        
        -------
        Block Matrix Mult
        Execute time: 0.157646
        RMSE: 0.0000
        
        real    0m1.821s
        user    0m1.649s
        sys     0m0.190s
    ```
3. Performance Results 256x256
    ```
        Batch Run for 256x256
        -------
        Naive Algorithm
        Execute time: 0.136247
        RMSE: 0.0005
        
        real    1m19.179s
        user    1m18.869s
        sys     0m0.260s
        
        -------
        Seperable Algorithm
        Execute time: 2.77681
        RMSE: 0.0000
        
        real    0m13.108s
        user    0m12.896s
        sys     0m0.220s
        
        -------
        Matrix Mult
        Execute time: 0.511993
        RMSE: 0.0000
        
        real    0m2.242s
        user    0m1.999s
        sys     0m0.260s
        
        -------
        Block Matrix Mult
        Execute time: 1.24782
        RMSE: 0.0000
        
        real    0m2.991s
        user    0m2.776s
        sys     0m0.230s
    ```
4. Performance Results 384x384
    ```
        Batch Run for 384x384
        -------
        Naive Algorithm
        Execute time: 2.4295
        RMSE: 0.0012
        
        real    6m39.421s
        user    6m38.859s
        sys     0m0.250s
        -------
        Seperable Algorithm
        Execute time: 4.21793
        RMSE: 0.0000
        
        real    0m40.436s
        user    0m40.207s
        sys     0m0.220s
        -------
        Matrix Mult
        Execute time: 1.8085
        RMSE: 0.0000
        
        real    0m3.656s
        user    0m3.454s
        sys     0m0.221s
        -------
        Block Matrix Mult
        Execute time: 4.25361
        RMSE: 0.0000
        
        real    0m6.109s
        user    0m5.895s
        sys     0m0.230s
    ```
