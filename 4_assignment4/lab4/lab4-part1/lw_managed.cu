#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <string>

__global__ void myKernel(int *m, int *v, int *r){
    r[threadIdx.x]  = v[0] * m[threadIdx.x*3+0];
    r[threadIdx.x] += v[1] * m[threadIdx.x*3+1];
    r[threadIdx.x] += v[2] * m[threadIdx.x*3+2];
}

int main(int argc, char* argv[]){

    uint size_m = 9*sizeof(int);
    uint size_v = 3*sizeof(int);
    int *m, *v, *r;
    cudaMallocManaged(&m, size_m);
    cudaMallocManaged(&v, size_v);
    cudaMallocManaged(&r, size_v);
    
    m[0]=0; m[1]=1; m[2]=2;
    m[3]=2; m[4]=3; m[5]=4;
    m[6]=4; m[7]=5; m[8]=6;

    v[0]=-1; v[1]=0; v[2]=1;

    uint b = 1; // dim3 b(1,1,1);
    uint t = 3; //dim3 t(3,1,1);
    myKernel<<<b,t>>>(m, v, r);
    cudaDeviceSynchronize(); 
    int scs = 1;
    for(uint i=0; i<3; i++){
        if(r[i] != 2){
            printf("error! ");
            scs = 0;
        }
        printf("r[%d] = %d\n", i, r[i]);
    }
    if(scs == 1){
        printf("Done with no error\n");
    }
    cudaFree(m);
    cudaFree(v);
    cudaFree(r);
    return 0;
}
