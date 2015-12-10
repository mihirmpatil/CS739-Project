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
unsigned long long int minsize;
unsigned long long int maxsize;
unsigned long long int incsize;

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

  unsigned long long int totalsize;
  unsigned long long int chunksize;

  chunksize = minsize;

  while(chunksize <= maxsize){

    totalsize = (unsigned long long int)(1024)*(unsigned long long int)(1024)*chunksize;
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
    printf("Read_size_runs_time:\t%llu\t%d\t%llu\n", chunksize, runs, (long long unsigned int) getTimeDiff(start,end));

    free(mem);

    chunksize += incsize;
  }

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
  
  unsigned long long int totalsize;
  unsigned long long int chunksize;

  chunksize = minsize;

  while(chunksize <= maxsize){

    totalsize = (unsigned long long int)(1024)*(unsigned long long int)(1024)*chunksize;
    mem = (char*)malloc(totalsize*sizeof(char));
    
    // Populate the memory continuously in chunks of 1KB
    clock_gettime(CLOCK_MONOTONIC, &start);
    for(i=0;i<runs;i++){
      for(j=0;j<totalsize;j++){
	mem[j] = rand()%26;
      }
    }
    clock_gettime(CLOCK_MONOTONIC,&end);
    printf("Write_size_runs_time:\t%llu\t%d\t%llu\n", chunksize, runs, (long long unsigned int) getTimeDiff(start,end));
  
    free(mem);
    chunksize += incsize;
  }
  
  return NULL;
}

int main(int argc, char *argv[]){

  if(argc != 7){
    printf("Usage: <executable> <number of threads> <min chunksize in mb> <max chunksize in mb> <increments in mb> <operation> <number of runs>\nOperation:\n1 - Write once and read repeatedly\n2 - Write continuously\n3 - Mix both read and write\n");
    return 0;
  }

  int i,ops;
  int threads = atoi(argv[1]);
  
  minsize = (atoi(argv[2]));//*(unsigned long long int)(1024)*(unsigned long long int)(1024);
  maxsize = (atoi(argv[3]));//*(unsigned long long int)(1024)*(unsigned long long int)(1024);
  incsize = (atoi(argv[4]));//*(unsigned long long int)(1024)*(unsigned long long int)(1024);
  ops = atoi(argv[5]);
  runs = atoi(argv[6]);
  
  //printf("Starting thread:%d chunksize:%d runs:%d ops:%d\n",threads,minsize,runs,ops);
  
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
