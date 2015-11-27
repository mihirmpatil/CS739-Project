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
	}
	uint64_t diff;
	struct timespec start, end;
	char filename[1024];
	strcpy(filename, argv[1]);

	int fd = open(filename, O_RDONLY);
	char buffer[BUFFER_SIZE];
	int total_read=0, num_read=0, times=0;

	int i, iterations = 10;

	for(i=0; i<iterations; i++)
	{
		total_read = 0;
		num_read = 0;
		lseek(fd, 0, SEEK_SET);
		clock_gettime(CLOCK_REALTIME, &start);
		while((num_read = read(fd, buffer, BUFFER_SIZE)) > 0)
		{
			total_read += num_read;
			times++;
		}
		clock_gettime(CLOCK_REALTIME, &end);
		diff = getTimeDiff(start, end);
		printf("Time for read of %d bytes from %s - %llu\n", total_read, filename, (long long unsigned int)diff);
		//system("echo 3 | sudo tee /proc/sys/vm/drop_caches");
		system("sudo sh -c \"sync; echo 3 > /proc/sys/vm/drop_caches\"");
	}
	return 0;
}
