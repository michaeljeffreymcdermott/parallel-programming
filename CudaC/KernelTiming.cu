#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>
#include <cuda_runtime.h>

__global__ void empty() {}
 
int main() {
  float ms;
  cudaEvent_t start, end;     //make event vars
  cudaEventCreate(&start);    //create start event
  cudaEventCreate(&end);      //create end event
  cudaEventRecord(start, 0);  //start recording
  empty<<< 1,1 >>> (); 
  cudaEventRecord(end, 0);    //end recording
  cudaEventSynchronize(end);  //sync, you have to do this!
  cudaEventElapsedTime(&ms, start, end); //get elapsed time, put in timing var
  printf("GPU = %.20lf\n", ms); //print timing
