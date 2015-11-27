/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, clilen;
     char buffer[524289];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
	signed int total_length  = 10485760*5; // 50 MB!
	signed int total_length_saved  = total_length;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
 //int message_length = atoi(argv[2]);	
	//	printf("total length = %d message length = %d\n",total_length,message_length);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     
		 while(1)
		 {
		 newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
     bzero(buffer,524289);
		// int i = total_length/message_length;
		 int total_recvd = 0;
		 while(total_recvd < total_length_saved)
		 {
  //   	do{
				n = read(newsockfd,buffer,524289);
				total_recvd += n;
//			}	while(n < message_length);
		//	printf("reading -> recvd till now = %d\n",total_recvd);
			//i--;
/*			if(strlen(buffer) < 1)
				break;*/
		 }
		printf("====here=====\n");
     if (n < 0) error("ERROR reading from socket");
     //printf("Received message: %s\n",buffer);
     n = write(newsockfd,"A",1);
     if (n < 0) error("ERROR writing to socket");
		 }
     return 0; 
}
