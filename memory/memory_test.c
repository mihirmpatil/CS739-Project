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

int runs;
int totalsize;
int chunksize;

uint64_t getTimeDiff(struct timespec start,struct timespec end)
{
  uint64_t diff = end.tv_sec-start.tv_sec;
  diff = diff * BILLION;
  diff = diff + end.tv_nsec - start.tv_nsec;
  return diff;
}

/* 
 * Allocate memory, write once and keep reading
 */
void* allocAndRead(void *args){

  char *mem;
  int i,j,temp;
  uint64_t diff;
  struct timespec start, end;
  
  printf("AllocAndRead Starting thread: %d\n",((targs*)(args))->tid);

  mem = (char*)malloc(totalsize*sizeof(char));

  // Populate the memory in 1MB chunks
  for(i=0;i<chunksize;i++){
    memset(mem+(i*1024*1024),rand()%26,sizeof(char)*1024*1024);
  }
  
  // Read for the specified number of times
  clock_gettime(CLOCK_MONOTONIC, &start);
  for(i=0;i<runs;i++){
    for(j=0;j<totalsize;j++){
      temp = (temp+mem[j])%26;
    }
  }
  clock_gettime(CLOCK_MONOTONIC,&end);
  printf("alloc and read elapsed time = %llu nanoseconds\n", (long long unsigned int) getTimeDiff(start,end));
  
  //printf("Printing to avoid compiler optimization: %d\n",temp);
  printf("Ending thread: %d\n",((targs*)(args))->tid);

  free(mem);
  return NULL;
}


/*
 * Allocate memory and write for the specified number of times.
 * Finally read the data once to avoid unreferenced memory
 */
void* allocAndWrite(void *args){

  char *mem;
  int i,j,temp = 0,rno;
  uint64_t diff;
  struct timespec start, end;
  
  printf("AllocAndWrite Starting thread: %d\n",((targs*)(args))->tid);

  mem = (char*)malloc(totalsize*sizeof(char));

  // Populate the memory continuously in chunks of 1KB
  clock_gettime(CLOCK_MONOTONIC, &start);
  for(i=0;i<runs;i++){
    for(j=0;j<totalsize;j++){
      mem[j] = rand()%26;
    }
  }
  clock_gettime(CLOCK_MONOTONIC,&end);
  printf("alloc and write elapsed time = %llu nanoseconds\n", (long long unsigned int) getTimeDiff(start,end));

  // Read the entire data
  /*for(i=0;i<totalsize;i++)
    temp = (temp + mem[i])%26;*/
  
  //printf("Printing to avoid compiler optimization: %d\n",temp);
  printf("Ending thread: %d\n",((targs*)(args))->tid);
  
  free(mem);
  
  return NULL;
}

int main(int argc, char *argv[]){

  if(argc != 5){
    printf("Usage: <executable> <number of threads> <chunksize in mb> <operation> <number of runs>\nOperation:\n1 - Write once and read repeatedly\n2 - Write continuously\n3 - Mix both read and write\n");
    return 0;
  }

  int i,ops;
  int threads = atoi(argv[1]);
  
  totalsize = atoi(argv[2])*1024*1024;
  runs = atoi(argv[4]);
  ops = atoi(argv[3]);
  chunksize = atoi(argv[2]);
  
  printf("Starting thread:%d chunksize:%d runs:%d ops:%d\n",threads,totalsize/(1024*1024),runs,ops);
  
  pthread_t pthreads[MAX_THREADS];
  targs threadarguments[MAX_THREADS];
  
  for(i=0;i<threads;i++){
    threadarguments[i].tid = i+1;
    switch(ops){
    case 1:
      pthread_create(&pthreads[i],NULL,allocAndRead,&(threadarguments[i]));
      break;
    case 2:
      pthread_create(&pthreads[i],NULL,allocAndWrite,&(threadarguments[i]));
      break;
    case 3:
      if(i&1)
	pthread_create(&pthreads[i],NULL,allocAndWrite,&(threadarguments[i]));
      else
	pthread_create(&pthreads[i],NULL,allocAndRead,&(threadarguments[i]));
      break;
    default:
      break;
    }
  }
  
  pthread_exit(0);	
  
  return 0;
}
