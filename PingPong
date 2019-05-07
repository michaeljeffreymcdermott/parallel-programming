#include <pthread.h>
 
void ping(void);  //forward declaration for function
void pong(void);  //forward declaration for function
 
int loops=0;      //global variable, it doesn't necessarily have to be in main()
int number=1;     //global variable
int pingpong=0;   //global variable
pthread_mutex_t mutex;  //mutex lock declaration
 
//main function. Please do not ever declare a function like this.
//this is just to exemplify how lenient C99 can be sometimes.
//notice that the main function has no return type, nor does it take args
//This is technically fine, just REALLY bad form.
//normally this looks like:
//int main(int argc, char* argv) {}
 
main() {
  pthread_t pinger, ponger;
  pthread_mutex_init(&mutex, NULL);
 
  pthread_create( &pinger, NULL, (void*)&ping, NULL );
  pthread_create( &ponger, NULL, (void*)&pong, NULL );
 
  pthread_join( pinger, NULL );
  pthread_join( ponger, NULL );
 
  printf("%d\n",loops);
}
 
void ping(void) {
  int i; //for iterator
  for (i=0; i<100; i++) {
    pthread_mutex_lock( &mutex );
    if( pingpong ) {
     printf("%4d - %1d - ping\n", number, i);
     number ++;
     pingpong = 0;
    } else {
     i--;
    }
    pthread_mutex_unlock( &mutex );
    loops++;
  }
  pthread_exit(0);
}
 
void pong(void) {
  int i; //for iterator
  for (i=0; i<100; i++) {
    pthread_mutex_lock( &mutex );
    if( !pingpong ) {
     printf("%4d - %1d - pong\n", number, i);
     number ++;
     pingpong = 1;
    } else {
     i--;
    }
    pthread_mutex_unlock( &mutex );
    loops++;
  }
  pthread_exit(0);
}
