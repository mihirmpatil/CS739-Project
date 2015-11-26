#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <fcntl.h>
#include <string.h>

#define BUFFER_SIZE 4096
#define BILLION 1000000000L

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
	strcpy(argv[1], filename);

	int fd = open(filename, O_RDONLY);
	char buffer[BUFFER_SIZE];
	int total_read=0, num_read=0, times=0, iterations=100;

	clock_gettime(CLOCK_REALTIME, &start);
	while(num_read = read(fd, buffer, BUFFER_SIZE) > 0)
	{
		total_read += num_read;
		times++;
/*
		if(times == iterations)
			break;
*/
	}
	clock_gettime(CLOCK_REALTIME, &end);
	diff = time_diff(start, end);
	printf("Time for read of %s - %llu\n", filename, (long long unsigned int)diff);

	return 0;
}
