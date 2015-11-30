#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <fcntl.h>
#include <string.h>

#define BUFFER_SIZE 4096
#include "getTimeDiff.c"

uint64_t time_diff(struct timespec start, struct timespec end)
{
	return BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
}

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("Usage: binary filename\n");
		exit(1);
	}
	srand(time(NULL));
	uint64_t diff;
	struct timespec start, end;
	char filename[1024];
	strcpy(filename, argv[1]);

	int fd = open(filename, O_WRONLY);
	int sz = lseek(fd, 0L, SEEK_END);
	lseek(fd, 0L, SEEK_SET);
	char buffer[BUFFER_SIZE];
	int i;
	for(i=0; i<BUFFER_SIZE-1; i++)
		buffer[i] = 'b';
	buffer[BUFFER_SIZE] = '\0';

	int iterations = 10;
	int times, random_num;

	for(i=0; i<iterations; i++)
	{
		times = 0;
		clock_gettime(CLOCK_REALTIME, &start);
		random_num = rand()%(sz-BUFFER_SIZE);

		if(pwrite(fd, buffer, BUFFER_SIZE, random_num) < 0)
		{
			perror("pwrite failed\n");
			close(fd);
			exit(1);
		}
		fsync(fd);
		clock_gettime(CLOCK_REALTIME, &end);
		times++;
		diff = getTimeDiff(start, end);
		printf("Random write performed %d times on %s\t", times, filename);
		printf("%llu\n", (long long unsigned int)diff);
		//system("sudo sh -c \"sync; echo 3 > /proc/sys/vm/drop_caches\"");
	}
	return 0;
}
