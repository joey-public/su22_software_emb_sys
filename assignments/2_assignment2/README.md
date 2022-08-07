## WES 237B Assignment 2

## Part 1: Welcome to PYNQ
### Compile and Run Instructions
```
    cd 1_lab_pynq_basics
    make
    bin/vectest
```
### Description
For this part of the assignment I first installed the Eigen C++ library. I used the Eignen `vector` type and implemented custom dot and cross product functions, in the `vecDot.cxx` and `vecCross.cxx` files. Then in the `vecMultTest.cxx` file I tested the results of my custom functions to the built in dot and cross product functions within the Eigen library. 
### Deliverables
*None*

In part one of this assignment I 
## Part 2: Welcome to OpenCV
### Compile and Run Instructions
```
    cd 2_dct 
    make
    bin/dcttest arg
```
**Note arg should be `0` for naive dct. and `1` for seperable dct.**
### Description

## Part 3: Matrix Multiplication and Performance
### Compile and Run Instructions
```
    cd 3_matMult
    make
    bin/matMultTest
```
### Description

## Part 4: 2D DCTs
### Compile and Run Instructions
```
    cd 4_dct2d
    make
    bin/dct2d width height
```
**Note `width` and `height` specify the size to rescale the image to. For the code to work as expected `width` and `height` should be the same value and be divisible by 8. passing no args will default to 64x64. Passing just the `width` will make `height=width`.**

### Description
