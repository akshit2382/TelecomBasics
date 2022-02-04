/*
filename server_ipaddress portno

argv[0] filename
argv[1] server_ipaddress
argv[2] portno

*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

void error(const char *msg)
{
	perror(msg);
	exit(0);
}

int main(int argc, char *argv[])
{
	int sockfd, portno, n, data_len;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	char buffer[255];
	if(argc<3){
		fprintf(stderr, "usage %s hostname port\n", argv[0]);
		exit(1);
	}
	portno  = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
		error("Error opening socket");

	server = gethostbyname(argv[1]);
	if(server == NULL)
	{
		fprintf(stderr, "Error, no such host");
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0)
		error("Connection failed");

	bzero(buffer, 255);
	fgets(buffer, 255, stdin);
	data_len = sizeof(buffer);
	n=send(sockfd,(char *)&data_len,sizeof(data_len),0);
	if(n<0)
	{
		printf("Error in Sending Size\n");
		exit(1);
	}
	n = send(sockfd, buffer, strlen(buffer),0);
	if(n < 0)
		error("Error on writing");


	close(sockfd);
	return 0;
}

