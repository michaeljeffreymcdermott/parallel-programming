/* 
This is an good trick to see what is going on for debugging purposes. It is terribly BAD
to print from GPU kernels in anything you want to be performance oriented though. It is not a
performance oriented feature!
*/
#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>
#include <cuda_runtime.h>
 
__global__ void kernel()
{
	/* 
           this just gets some kernel specific parameters
	   this is just so you can see how non-deterministic thread timing is
	*/
        int tidx = threadIdx.x + blockIdx.x * blockDim.x;
	int tidy = threadIdx.y + blockIdx.y * blockDim.y;
 
	/* print some stuff out */
	int size = sizeof(int);
	printf("Hello, World! size=%d   tidx=%d, tidy=%d\n", size, tidx, tidy);
	return;
}
 
int main(int argc, char** argv)
{
        /*
           Keep this in mind. in Cuda 8 compute 2.0 was deprecated and it may be 
           removed by now. CDER only currently (11/2018) supports Cuda 7 so the below
           will work and may or may not warn you about this.
        */
	printf("You compile this with 'nvcc -arch sm_20 hello.cu -o hello'\n");
	printf("You need -at least- arch of sm_20 to print from kernels\n");
	dim3 dimBlock( 16, 16, 1 );
	dim3 dimGrid( 16, 16, 1 );
 
	kernel<<<dimGrid,dimBlock>>>();
	cudaDeviceSynchronize();  /* you also -need- this here to print from the kernel */
	return 1;
}
