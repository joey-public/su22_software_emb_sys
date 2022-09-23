#!/bin/bash

echo "Batch Run for 384x384"
time bin/dct2d 0 384
time bin/dct2d 1 384
time bin/dct2d 2 384
time bin/dct2d 3 384
