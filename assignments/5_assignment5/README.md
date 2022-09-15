# Lab 5 Report

## Compile and Run Instructions
### Python Tensor flow example
```
python3 lab5/tf_mnist_example.py
```
### Lab 5 GPU NN 
The instructions will run the `AND` gate example
```
cd lab5
make
./lab5
```
To run the `XNOR_GATE` you must edit `lab5/src/dataset.cu` to uncomment line 3. then remake the project and run.  
## Final Code
All final Code it in the lab5 directory.
## Reuslts for AND Gate
'''
Cost : 0.046890
Total number of epochs : 4999
Data : [0.000000, 0.000000] / Pred (pred) - Real : 0.048753 (0.000000) - 0.000000
Data : [1.000000, 0.000000] / Pred (pred) - Real : 0.176688 (0.000000) - 0.000000
Data : [0.000000, 1.000000] / Pred (pred) - Real : 0.176613 (0.000000) - 0.000000
Data : [1.000000, 1.000000] / Pred (pred) - Real : 0.754628 (1.000000) - 1.000000
Data : [0.000000, 0.000000] / Pred (pred) - Real : 0.048753 (0.000000) - 0.000000
Data : [1.000000, 0.000000] / Pred (pred) - Real : 0.176688 (0.000000) - 0.000000
Data : [0.000000, 1.000000] / Pred (pred) - Real : 0.176613 (0.000000) - 0.000000
Data : [1.000000, 1.000000] / Pred (pred) - Real : 0.754628 (1.000000) - 1.000000
Data : [0.000000, 0.000000] / Pred (pred) - Real : 0.048753 (0.000000) - 0.000000
Data : [1.000000, 0.000000] / Pred (pred) - Real : 0.176688 (0.000000) - 0.000000
Data : [0.000000, 1.000000] / Pred (pred) - Real : 0.176613 (0.000000) - 0.000000
Data : [1.000000, 1.000000] / Pred (pred) - Real : 0.754628 (1.000000) - 1.000000
Data : [0.000000, 0.000000] / Pred (pred) - Real : 0.048753 (0.000000) - 0.000000
Data : [1.000000, 0.000000] / Pred (pred) - Real : 0.176688 (0.000000) - 0.000000
Data : [0.000000, 1.000000] / Pred (pred) - Real : 0.176613 (0.000000) - 0.000000
Data : [1.000000, 1.000000] / Pred (pred) - Real : 0.754628 (1.000000) - 1.000000

Accuracy: 1

real    0m9.029s
user    0m3.760s
sys 0m3.088s
'''
## Results for XNOR_GATE
'''
Cost : 0.134367
Total number of epochs : 4999
Data : [0.000000, 0.000000] / Pred (pred) - Real : 0.841592 (1.000000) - 1.000000
Data : [1.000000, 0.000000] / Pred (pred) - Real : 0.291635 (0.000000) - 0.000000
Data : [0.000000, 1.000000] / Pred (pred) - Real : 0.311149 (0.000000) - 0.000000
Data : [1.000000, 1.000000] / Pred (pred) - Real : 0.482373 (0.000000) - 1.000000
Data : [0.000000, 0.000000] / Pred (pred) - Real : 0.841592 (1.000000) - 1.000000
Data : [1.000000, 0.000000] / Pred (pred) - Real : 0.291635 (0.000000) - 0.000000
Data : [0.000000, 1.000000] / Pred (pred) - Real : 0.311149 (0.000000) - 0.000000
Data : [1.000000, 1.000000] / Pred (pred) - Real : 0.482373 (0.000000) - 1.000000
Data : [0.000000, 0.000000] / Pred (pred) - Real : 0.841592 (1.000000) - 1.000000
Data : [1.000000, 0.000000] / Pred (pred) - Real : 0.291635 (0.000000) - 0.000000
Data : [0.000000, 1.000000] / Pred (pred) - Real : 0.311149 (0.000000) - 0.000000
Data : [1.000000, 1.000000] / Pred (pred) - Real : 0.482373 (0.000000) - 1.000000
Data : [0.000000, 0.000000] / Pred (pred) - Real : 0.841592 (1.000000) - 1.000000
Data : [1.000000, 0.000000] / Pred (pred) - Real : 0.291635 (0.000000) - 0.000000
Data : [0.000000, 1.000000] / Pred (pred) - Real : 0.311149 (0.000000) - 0.000000
Data : [1.000000, 1.000000] / Pred (pred) - Real : 0.482373 (0.000000) - 1.000000

Accuracy: 0.75

real	0m9.502s
user	0m3.924s
sys	0m3.188s
'''
