#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <stdint.h>

#define BILLION 1000000000L
#define MAX_THREADS 64

struct t_args{
	int tid;
};

typedef struct t_args targs;

int precision;

uint64_t getTimeDiff(struct timespec start,struct timespec end)
{
  uint64_t diff = end.tv_sec-start.tv_sec;
  diff = diff * BILLION;
  diff = diff + end.tv_nsec - start.tv_nsec;
  return diff;
}

/* 
 * Borwein calculation of PI 
 */
void* calculatePi(void *args){
  int tid;
  double p16 = 1, pi = 0;
  unsigned long long int k;
  uint64_t diff;
  struct timespec start, end;
  
  tid = ((targs*)(args))->tid;
  printf("Starting thread: %d\n",tid);

  clock_gettime(CLOCK_MONOTONIC, &start);
  for(k=0; k<=precision; k++){
    pi += 1.0/p16 * (4.0/(8*k + 1) - 2.0/(8*k + 4) - 1.0/(8*k + 5) - 1.0/(8*k+6));
    p16 *= 16;
  }
  clock_gettime(CLOCK_MONOTONIC,&end);
  printf("Pi calculation elapsed time = %llu nanoseconds\n", (long long unsigned int) getTimeDiff(start,end));

  printf("Ending thread: %d\n",tid);

  return NULL;
}

int main(int argc, char *argv[]){

	if(argc != 3){
		printf("Usage: <executable> <number of bits of precision> <number of threads>\n");
		return 0;
	}
	
	precision = atoi(argv[1]);
	int threads = atoi(argv[2]);
	int i;

	printf("Calculating Pi with precision: %d, threads: %d\n",precision,threads);

	pthread_t pthreads[MAX_THREADS];
	targs threadarguments[MAX_THREADS];

	for(i=0;i<threads;i++){
		threadarguments[i].tid = i+1;
		pthread_create(&pthreads[i],NULL,calculatePi,&(threadarguments[i]));
	}

	pthread_exit(0);	
	
 	return 0;
}
