#!/bin/bash
make clean
make
time ./hw3 1 1080 1080
make clean
make
time ./hw3 2 1080 1080
make clean
make
time ./hw3 3 1080 1080
