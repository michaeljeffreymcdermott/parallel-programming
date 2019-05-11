/* 
Hello World in Cuda-C
filename: hello_world.cu
notice the .cu extension! THIS IS IMPORTANT!
*/
#include <stdlib.h>
#include <stdio.h>
#include <cuda.h>
#include <cuda_runtime.h>
 
__global__ hellokernel(void) {}
 
int main() {
    hellokernel<<<1,1>>>();
    printf("Hello World!\n");
    return;
}
