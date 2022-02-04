#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<stdio.h>

void error(char *);

int main(int argc, char *argv[])
{
	int sock, length, n;
	struct sockaddr_in server, from;
	//struct hostent *hp;
	char buffer[256];

	if(argc != 2)
	{
		printf("Usage: server port\n");
		exit(1);
	}

	sock = socket(AF_INET, SOCK_DGRAM, 0);

	if(sock < 0)
	{
		error("socket");
	}
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[1]));
	server.sin_addr.s_addr=INADDR_ANY;
	length = sizeof(struct sockaddr_in);

	printf("Please enter the message: ");

	bzero(buffer, 256);
	fgets(buffer, 255, stdin);
	n = sendto(sock, buffer, strlen(buffer), 0, &server, length);
	if(n < 0)
	{
		error("Sendto");
	}

	close(sock);
}
