#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/mman.h>
#include <string.h>

int main(int argc, char *argv[])
{
  if(argc != 2){
    printf("\nUsage: <executable> <sizeingb>\n");
  }
  int sizeingb = atoi(argv[1]);
	long long unsigned int to_allocate = (long long unsigned int)sizeingb*1024*1024*1024;
	char *ptr = (char *)malloc(to_allocate*sizeof(char));
	if (ptr == NULL)
	{
		perror("ptr is NULL\n");
		exit(1);
	}
	memset(ptr, 0, to_allocate);
	mlock(ptr, to_allocate);
	printf("LOCKED!\n");
	char a;
	long long unsigned int x=0;
	while(1)
	{
		*(ptr + x) = 'a';
		x = ++x % to_allocate;
		//printf("x=%llu\n",x);
	}

	return 0;
}
