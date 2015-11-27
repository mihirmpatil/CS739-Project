#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <stdint.h>

#define BILLION 1000000000L
#define MAX_THREADS 64
#define MAX_LOCKS 128

pthread_mutex_t locks[MAX_LOCKS];

struct t_args{
	int tid;
};

typedef struct t_args targs;

unsigned long long int globalv = 0;
long int locking_parameter;

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
void* incrementGlobal(void *args){
  int tid;
  long int k;
  uint64_t diff;
  struct timespec start, end;
  
  tid = ((targs*)(args))->tid;
  //printf("Starting thread: %d\n",tid);

  clock_gettime(CLOCK_MONOTONIC, &start);
  
  for(k=0; k<=locking_parameter; k++){
    pthread_mutex_lock(&locks[0]);
    globalv++;
    pthread_mutex_unlock(&locks[0]);
  }
  
  clock_gettime(CLOCK_MONOTONIC,&end);
  printf("%d %llu\n",tid, (long long unsigned int) getTimeDiff(start,end));

  //printf("Ending thread: %d\n",tid);

  return NULL;
}

int main(int argc, char *argv[]){

  if(argc != 3){
    printf("Usage: <executable> <number of times lock should be obtained> <number of threads>\n");
    return 0;
  }
  
  locking_parameter = atol(argv[1]);
  int threads = atoi(argv[2]);
  int i;
  
  printf("Locking times: %ld, threads: %d\n",locking_parameter,threads);
  
  pthread_t pthreads[MAX_THREADS];
  targs threadarguments[MAX_THREADS];

  for(i=0;i<threads;i++){
    threadarguments[i].tid = i+1;
    pthread_create(&pthreads[i],NULL,incrementGlobal,&(threadarguments[i]));
  }
  
  pthread_exit(0);	
  
  return 0;
}
