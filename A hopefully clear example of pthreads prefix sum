/* parallel prefix sum.
   this only works correctly for arrays of size power of 2
   it is currently shackled so that we do not run into problems
   with allocating too many or two few threads.
*/
 
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h> /* for random seed */
#include <argp.h>
#include <math.h>
 
/* setting defaults for program */
#define SIZEMIN 8
#define SIZEMAX 4096
#define THREADS SIZE
#define TYPE 1
#define VERBOSE 0
 
/* argparse variable, builtin to argparse */
const char *argp_program_version = "1.0";
 
/* argparse variable, builtin to argparse */
const char *argp_program_bug_address = "mmcdermott2@student.gsu.edu";
 
/* 
  global struct to hold arguments, convenient and makes moving things between
  argarse and main easier.
*/
struct arguments {
    char *args[2];  /* ARG1 and ARG2 */
    int verbose;
    int type, size, threads, last;
};
 
/* documentation for program */
static char doc[] = "Pthreads example program for prefix sum"; 
/* description of args */
static char args_doc[] = "verbose type"; 
 
/* {name, key, arg, flags, doc} */
static struct argp_option options[] = {
    {"verbose", 'v', 0, 0, "Verbose output. (deprecated)"},
    {"size",    's', "tmpsize", 0, "Size of the array to perform prefix sum on. Input the n from 2^n where n is 3 to 12."},
    {"type",    't', "tmptype", 0, "Type of array, 0 (zeros), 1 (ones), or 2 (random/other)."},
    {0}
};
 
/* 
  argparse fuction, holds the case statement that deals with what each arg does
  I also usually deal with whatever arg checking I need to here as its a relatively
  isolated place to do it and keeps main from becoming overly bloated with lots of checks.
 
  If the check is overly complex I usually break it out into its own function for clarity.
*/
static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = (*state).input;
    switch(key) {
        case 'v':
            (*arguments).verbose = 1;
            /* 
               (*foo).bar and foo->bar
               Are generally considered equivalent, barring someone overloading an operator. 
 
               The first is possibly more correct in C99, the second avoids forgetting
               operartor binding precedence and having to spend hours hunting down a mentally 
               invisible error in favor of mixing in some C++ notation. I am using the first
               here, but you can use whatever is simpler.
            */
            break;
        case 's':
            (*arguments).size = 1<<atoi(arg);
            if((*arguments).size < SIZEMIN || (*arguments).size > SIZEMAX) {
                printf("Size of array MUST be greater than 2^3 and less than 2^12! Exiting!\n");
                exit(0);
            }
            (*arguments).threads = (*arguments).size;
            break;
        case 't':
            (*arguments).type = atoi(arg);
            break;
        case ARGP_KEY_ARG:
            if((*state).arg_num >=2) /* too many args */
                argp_usage(state);
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;    
}
 
/*  another struct needed for argparse  */
static struct argp argp = {options, parse_opt, args_doc, doc};
 
/* 
  forward declaration for function pointers
  This is just lets me put things at the end of the file
  You can also just do a full declaration here for simplicity.
 
  NOTE: if you add parameters to your function declarations 
  you have to also change them everywhere else!
*/
void *downsweep(void *);
void *upsweep(void *);
void *shift(void *);
void printarr(char *, int *, int);
unsigned long upperpow(unsigned long);
 
/* barrier that all my threads will hit and wait at */
pthread_barrier_t barrier;
 
/* struct to pass to threads */
typedef struct datastruct {
  int *arr;
  int threads;
  int size;
  int tid;
  int last;
} datastruct;
 
int main(int argc, char **argv) {
    struct arguments arguments;
    int i, err;
 
    /* get defaults */
    arguments.verbose = VERBOSE;
    arguments.size = SIZEMIN;
    arguments.type = TYPE;
    arguments.threads = SIZEMIN/2;
    arguments.last = -1;
 
    /* parse args with argp */
    argp_parse(&argp, argc, argv, 0, 0, &arguments);
 
    printf("verbose=%d, size=%d, threads=%d, type=%d\n", 
        arguments.verbose, arguments.size, arguments.threads, arguments.type);
 
    /* declare our array to prefix sum */
    int *arr = (int *)malloc(sizeof(int) * arguments.size);
 
    /* declare our threads, its handy to make an array of them */
    pthread_t *upthreads    = (pthread_t *)malloc(sizeof(pthread_t) * arguments.threads);
    pthread_t *downthreads  = (pthread_t *)malloc(sizeof(pthread_t) * arguments.threads);
    pthread_t *shiftthreads = (pthread_t *)malloc(sizeof(pthread_t) * arguments.threads);
    srand(time(NULL)); /* seed the psuedo random number generator */
    for(int i = 0; i < arguments.size; i++){
        if(arguments.type == 0) {arr[i] = 0;} /* populate with zeros */
        else if(arguments.type == 1) {arr[i] = 1;} /* populated with ones */
        else {arr[i] = rand() % 10;} /* populated with random numbers */
    }
    // printarr("main1", arr, arguments.size);
 
    /* init the barrier, we can simply re-use this over and over! */
    pthread_barrier_init(&barrier, NULL, arguments.threads);
 
    /* Do the upsweep part of the prefix sum*/
    for(int i = 0; i < arguments.threads; i++) {
      struct datastruct *data = (datastruct *)malloc(sizeof(datastruct));
      /* here I use the other notation. its interchangeable */
      (*data).threads = arguments.threads;
      (*data).size = arguments.size;
      (*data).arr = arr;
      (*data).tid = i;
 
      if(arguments.verbose == 1) printf("Launching thread(%d) ", i);
      /* technically we could now not cast data to a void */
      /* err = pthread_create(&threads[i], NULL, upsweep, (void *)data); */
      err = pthread_create(&upthreads[i], NULL, upsweep, data); 
    }
 
    /* 
      join threads, the are now gone. you cannot reuse them once they exit!
      while technically this may work, its more or less not defined behavior.
      its best to just join them and make new threads.
    */
    for(i = 0; i < arguments.threads; i++) {
      pthread_join(upthreads[i], NULL);
    }
 
    arguments.last = arr[arguments.size - 1];
    arr[arguments.size - 1] = 0;
    // printarr("main2", arr, arguments.size);
 
 
    /* do the downsweep part of the prefix sum */
    for(int i = 0; i < arguments.threads; i++) {
      struct datastruct *data = (datastruct *)malloc(sizeof(datastruct));
      /* here I use the other notation. its interchangeable */
      (*data).threads = arguments.threads;
      (*data).size = arguments.size;
      (*data).arr = arr;
      (*data).tid = i;
 
      if(arguments.verbose == 1) printf("Launching thread(%d) ", i);
      /* technically we could now not cast data to a void */
      /* err = pthread_create(&threads[i], NULL, upsweep, (void *)data); */
      err = pthread_create(&downthreads[i], NULL, downsweep, data); 
    }
 
    /* join threads */
    for(i = 0; i < arguments.threads; i++) {
      pthread_join(downthreads[i], NULL);
    }
 
    /* do the downsweep part of the prefix sum */
    for(int i = 0; i < arguments.threads; i++) {
      struct datastruct *data = (datastruct *)malloc(sizeof(datastruct));
      /* here I use the other notation. its interchangeable */
      (*data).threads = arguments.threads;
      (*data).size = arguments.size;
      (*data).arr = arr;
      (*data).tid = i;
      (*data).last = arguments.last;
 
      if(arguments.verbose == 1) printf("Launching thread(%d) ", i);
      /* technically we could now not cast data to a void */
      /* err = pthread_create(&threads[i], NULL, upsweep, (void *)data); */
      err = pthread_create(&shiftthreads[i], NULL, shift, data); 
    }
 
    /* join threads */
    for(i = 0; i < arguments.threads; i++) {
      pthread_join(shiftthreads[i], NULL);
    }
 
 
 
    printarr("main3", arr, arguments.size);
 
    /* cleanup */
    printf("Destroying Barrier\n");
    pthread_barrier_destroy(&barrier);
    printf("Freeing Prefix Sum Array Allocation\n");
    free(arr);
    /*
      leaving this in causes a memory error on free()
      this is likely because it serves the same function as join.
      printf("Freeing Threads Array Allocation\n");
      free(threads);
    */
    return 0;
 
}
 
/*
  https://developer.nvidia.com/sites/all/modules/custom/gpugems/books/GPUGems3/elementLinks/39fig03.jpg
  because we passed a reference we do NOT cast here, 
  we just set our variable equals 
  note: data->tid is equivalent with (*data).tid here
*/
void *upsweep(void *ptr) {
  datastruct *data = ptr;
  int d, offset = 1;
  int tid = data->tid ;
  int top = (int)log2l(data->size);
  for(d = 0; d <= top; d++) { /* for each level  of the 'tree' */
    /* get indices */
    int a = offset * (2 * tid + 1) - 1;
    int b = offset * (2 * tid + 2) - 1;
    int tmpA, tmpB;
    /* do not read or write off end of array */
    if(a < data->size) {
      tmpA = data->arr[a];
      if(b < data->size) {
        tmpB = tmpA + data->arr[b];
      }
    }
    offset *= 2;
    pthread_barrier_wait(&barrier); /* wait at the barrier! */
    if(b < data->size) data->arr[b] = tmpB;
    pthread_barrier_wait(&barrier); /* wait at the barrier! */
    if(tid == 0) printarr("upsweep", data->arr, data->size);
  }
  return;
}
 
/* 
  https://developer.nvidia.com/sites/all/modules/custom/gpugems/books/GPUGems3/elementLinks/39fig04.jpg
*/
void *downsweep(void *ptr) {
  datastruct *data = ptr;
  int d, offset = (int)upperpow(data->size);
  int tid = data->tid;
  int top = (int)log2l(data->size);
  for(d = top; d >= 0; d--) { /* for each level of the 'tree' */
    /* get indices */
    int a = offset * (2 * tid + 1) - 1;
    int b = offset * (2 * tid + 2) - 1;
    int tmpA, tmpB, temp;
    /* do not read or write off end of array */
    if(a < data->size) {
      temp = data->arr[a];
      tmpA = temp;
      if(b < data->size) {
        tmpA = data->arr[b];
        tmpB = temp + tmpA;
      } else {
        /* we may be on an edge case where we put the data back in place */
        if(d == top)
          tmpB = temp;
      }
    }
    // if(tid == 1) printf("%d:%d: a[%d]=%d, b[%d]=%d\n", d, tid, a, tmpA, b, tmpB);
    offset /= 2;
 
    pthread_barrier_wait(&barrier);
    if(a < data->size) data->arr[a] = tmpA;
    if(b < data->size) data->arr[b] = tmpB;
    pthread_barrier_wait(&barrier);
    if(tid == 0) printarr("downsweep", data->arr, data->size);
  }
  return;
}
 
void *shift(void *ptr) {
  datastruct *data = ptr;
  int tid = data->tid;
  int i;
  int threadcount = data->threads;
  int j = 0;
  for(i = 0; i < data->size; i += threadcount) {
    int forwardidx = 1 + tid + threadcount * j;
    int idx = tid + threadcount * j;
    int local;
    if(forwardidx < data->size) {
      local = data->arr[forwardidx];
      j++;
    } else {
      local = data->last;
    }
    /* everyone reads first, waits on all the reads, then writes */
    pthread_barrier_wait(&barrier);
    if(idx < data->size) data->arr[idx] = local;
    pthread_barrier_wait(&barrier);
  }
 
 
}
 
/* these are just helper functions */
 
void printarr(char *from, int *arr, int size) {
  int i;
  printf("%s", from);
  for(i = 0; i < size; i++)
    printf(" array[%d]=%d,", i, arr[i]);
  printf("\n");
}
 
unsigned long upperpow(unsigned long i) {
  /* http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2 */
  i--;
  i |= i >> 1;
  i |= i >> 2;
  i |= i >> 4;
  i |= i >> 8;
  i |= i >> 16;
  i++;
  return i;
}
