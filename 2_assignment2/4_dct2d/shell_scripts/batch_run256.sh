#!/bin/bash

echo "Batch Run for 256x256"
time bin/dct2d 0 256
time bin/dct2d 1 256
time bin/dct2d 2 256
time bin/dct2d 3 256
