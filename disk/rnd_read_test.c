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
		printf("Usage: binary filename reads\n");
		exit(1);
	}
	srand(time(NULL));
	uint64_t diff;
	struct timespec start, end;
	char filename[1024];
	strcpy(filename, argv[1]);

	int fd = open(filename, O_RDONLY);
	int sz = lseek(fd, 0L, SEEK_END);
	lseek(fd, 0L, SEEK_SET);
	char buffer[BUFFER_SIZE];
	int k, num_reads=atoi(argv[2]);

	int i, iterations = 10;
	int times, random_num;

	for(i=0; i<iterations; i++)
	{
		times = 0;
		clock_gettime(CLOCK_REALTIME, &start);
		for(k=0; k<num_reads; k++)
		{
			random_num = rand()%(sz-BUFFER_SIZE);

			system("sudo sh -c \"sync; echo 3 > /proc/sys/vm/drop_caches\"");
			if(pread(fd, buffer, BUFFER_SIZE, random_num) < 0)
			{
				perror("pread failed\n");
				exit(1);
			}
			times++;
		}
		clock_gettime(CLOCK_REALTIME, &end);
		diff = getTimeDiff(start, end);
		printf("Random read performed %d times on %s\t", times, filename);
		printf("%llu\n", (long long unsigned int)diff);
		system("sudo sh -c \"sync; echo 3 > /proc/sys/vm/drop_caches\"");
	}
	return 0;
}
