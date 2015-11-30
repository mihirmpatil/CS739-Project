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
	if(argc != 3)
	{
		printf("Usage: binary filename filesize\n");
		exit(1);
	}
	uint64_t diff;
	struct timespec start, end;
	char filename[1024];
	strcpy(filename, argv[1]);

	char buffer[BUFFER_SIZE];
	int i;
	for(i=0; i<BUFFER_SIZE-1; i++)
		buffer[i] = 'a';
	buffer[BUFFER_SIZE] = '\0';
	int fd, total_written=0, num_written=0, times=0;
 	//int file_size=100*1024;											// This will be multiplied by BUFFER_SIZE
	int file_size = atoi(argv[2]);
	int k, iterations = 10;

	for(k=0; k<iterations; k++)
	{
		total_written = 0;
		num_written = 0;
		fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0640);
		if(fd == -1)
			perror("fd is -1");
		lseek(fd, 0, SEEK_SET);
		clock_gettime(CLOCK_REALTIME, &start);
		for(i=0; i<file_size; i++)
		{
			num_written = write(fd, buffer, BUFFER_SIZE);
			// fsync(fd);
			if (num_written != BUFFER_SIZE)
			{
				printf("Error=> bytes written %d, BUFFER_SIZE %d\n", num_written, BUFFER_SIZE);
				close(fd);
				exit(1);
			}
			total_written += num_written;
		}
		fsync(fd);
		clock_gettime(CLOCK_REALTIME, &end);
		diff = getTimeDiff(start, end);
		printf("Time for write of %s, %d bytes - %llu\n", filename, total_written, (long long unsigned int)diff);
		close(fd);
	}
	return 0;
}
