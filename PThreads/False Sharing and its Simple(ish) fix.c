#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
 
#define NUM_THREADS 4 
#define NUM_ITERATIONS (1L << 32) //LOTS of iterations
 
long *data; 
 
void *thread_function( void *args )
{
  long i;
  long j = (long)args;
 
  for ( i = 0; i < NUM_ITERATIONS; i++ )
    data[j] = i;
 
  return NULL;
}
 
int main( int argc, char **argv ) {
  long i, j;
  pthread_t ids[NUM_THREADS - 1];
  struct timeval start, end;
  double d;
 
  data = (long*)malloc( sizeof(long)*NUM_THREADS );
 
  for (j = 1; j <= NUM_THREADS; j++ ) {
    for(i = 0; i < NUM_THREADS; i++)
      data[i] = 0;
 
 
    gettimeofday( &start, NULL );
    for(i = 0; i < j-1; i++)
      pthread_create(ids+i, NULL, thread_function, (void*)i);
 
    thread_function( (void*)i );
 
    for(i = 0; i < j-1; i++)
      pthread_join( ids[i], NULL );
 
    gettimeofday(&end, NULL);
 
    d = end.tv_sec - start.tv_sec + (1.0e-6) * end.tv_usec - (1.0e-6) * start.tv_usec;
    printf("thread[%ld] %g tot_s %g iter/s\n", j, d, ((double)j * NUM_ITERATIONS) / d);
  }
  return 0;
}
