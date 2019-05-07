#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
 
pthread_mutex_t lock;
pthread_mutex_t condition_lock;
pthread_cond_t condition;
 
void *fun1();
void *fun2();
 
int count = 0;
 
#define DONE 10
#define STOP 3
#define HALT 6
 
int main() {
	pthread_t t1, t2;
 
	pthread_create(&t1, NULL, &fun1, NULL);
	pthread_create(&t2, NULL, &fun2, NULL);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	exit(0);
}
 
void *fun1() {
	// you can use while(1) or for(;;) here
	// I believe these are equivalent as they create loops without
	// predefined termination points
	while(1) { //potential infinite loop
		pthread_mutex_lock(&condition_lock);
		while(count >= STOP && count <= HALT)
			pthread_cond_wait(&condition, &condition_lock);
		pthread_mutex_unlock(&condition_lock);
 
		pthread_mutex_lock(&lock);
		printf("Counter = %d\n", ++count);
		pthread_mutex_unlock(&lock);
 
		if(count >= DONE) return;
	}
}
 
void *fun2() {
	while(1) { //potential infinite loop
		pthread_mutex_lock(&condition_lock);
		if(count < STOP || count > HALT)
			pthread_cond_signal(&condition);
		pthread_mutex_unlock(&condition_lock);
 
		pthread_mutex_lock(&lock);
		printf("counter = %d\n", ++count);
		pthread_mutex_unlock(&lock);
 
		if(count >= DONE) return;
	}
}
