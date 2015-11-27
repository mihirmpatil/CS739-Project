#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "getTimeDiff.c"

void error(char *msg)
{
    perror(msg);
    exit(0);
}

void rand_str(char *dest, size_t length)
{
    char charset[] = "0123456789"
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    while (length-- > 0)
    {
        size_t index = (double) rand() / RAND_MAX * (sizeof charset - 1);
        *dest++ = charset[index];
    }
    *dest = '\0';
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

		struct timespec start,stop;
		long accum;
		signed int total_length  = 10485760*5; // 50 MB!
		signed int total_length_saved  = total_length; 

    char buffer[256];
    bzero(buffer,256);
    if (argc < 4) {
       fprintf(stderr,"usage %s hostname port message_length\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    int message_length = atoi(argv[3]);
//		printf("total length = %d message length = %d\n",total_length,message_length);
    char *string = (char*)malloc(message_length*sizeof(char*));
    rand_str(string,message_length);

		clock_gettime(CLOCK_REALTIME, &start);
		int i = total_length/message_length;
		while(i>0)
		{
			//printf("writing -> i = %d\n",i);
    	n = write(sockfd,string,strlen(string));
			i--;
		}
		//printf("====here=====\n");
    if (n < 0) 
         error("ERROR writing to socket");
    n = read(sockfd,buffer,255);
		clock_gettime(CLOCK_REALTIME, &stop);
    if (n < 0) 
         error("ERROR reading from socket");
		if(strcmp("A",buffer)!=0) {
			printf("ERROR! WRITTEN STRING NOT SAME AS READ STRING\n");
			return(0);
		}
		accum = (stop.tv_sec - start.tv_sec)*1000000000 + (stop.tv_nsec - start.tv_nsec);
		uint64_t diff = getTimeDiff(start, stop);
		printf( "Time taken : %llu ns\n", (unsigned long long int)diff );
		printf( "Throughput : %lf Megabytes per s\n", (total_length_saved*1000.0/diff));

    //printf("%s\n",buffer);
    return 0;
}
