#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
 
#define THREAD_COUNT 40
 
pthread_barrier_t barrier;
 
void *fun(void *ptr) {
    int tid = (int)ptr;
    int w = rand() % 20;
 
    printf("thread %d sleeping for %ds\n", tid, w);
    sleep(w);
    printf("thread %d waiting on barrier sync\n", tid);
 
    pthread_barrier_wait(&barrier);
    printf("thread %d: exited barrier\n", tid);
    return ;
}
 
 
int main() {
    int i;
 
    /* this is how you allocate memory on the heap */
    pthread_t *thread_ids = (pthread_t *)malloc(sizeof(pthread_t) * THREAD_COUNT);
 
    /* create the barrier */
    pthread_barrier_init(&barrier, NULL, THREAD_COUNT);
 
    for (i=0; i < THREAD_COUNT; i++) {
        pthread_create(&thread_ids[i], NULL, fun, (void*)i);
    }
 
//    pthread_barrier_wait(&barrier);
 
    for (i=0; i < THREAD_COUNT; i++) {
        pthread_join(thread_ids[i], NULL);
    }
 
    /* destroy the barrier */
    pthread_barrier_destroy(&barrier);
 
    /* always free any memory you alloc */
    free(thread_ids);
    return 0;
}
