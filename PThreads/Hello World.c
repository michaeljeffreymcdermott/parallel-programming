#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
 
#define NUM_THREADS 1000
 
void *hello(void *threadid) {
   long tid;
   tid = (long)threadid; //thread ids are long ints
   printf("Hello from thread %ld\n", tid);
   pthread_exit(NULL);
}
 
int main(int argc, char *argv[]) {
   pthread_t threads_arr[NUM_THREADS];
   int rc;
   long t;
 
   for(t=0; t < NUM_THREADS; t++) {
     printf("Main: spawning(%ld)\n", t);
     rc = pthread_create(&threads_arr[t], NULL, hello, (void *)t);
     if (rc) {
       printf("ERROR; return code from pthread_create() is %d\n", rc);
       exit(-1);
       }
     }
 
   pthread_exit(NULL);
}
