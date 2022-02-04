#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<string.h>

int main(int argc, char *argv[])
{
	if(argc<3)
	{
		printf("Please Give 2 Port Numbers");
		exit(1);
	}

	int socket_desc1,socket_desc2,socket_desc3,socket_desc4,n,valread,ret,newsockfd1,newsockfd2,buffer_len;
	char buffer[1024]={0};
	struct sockaddr_in server1,server2,server3,server4,client;
	struct timeval tval;
	fd_set read_set, write_set;
	int maxfd;

	socket_desc1 = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(socket_desc1 == -1)
	{
		printf("socket 1");
		exit(1);
	}
	socket_desc2 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(socket_desc2 == -1)
	{
		printf("socket 2");
		exit(1);
	}
	socket_desc3 = socket(AF_INET,SOCK_STREAM,0);
	if(socket_desc3 == -1)
	{
		printf("socket 3");
		exit(1);
	}
	socket_desc4 = socket(AF_INET,SOCK_STREAM,0);
	if(socket_desc4 == -1)
	{
		printf("socket 4");
		exit(1);
	}

	server1.sin_family = AF_INET;
	server1.sin_port = htons(atoi(argv[1]));
	server1.sin_addr.s_addr = INADDR_ANY;

	server2.sin_family = AF_INET;
	server2.sin_port = htons(atoi(argv[2]));
	server2.sin_addr.s_addr = INADDR_ANY;

	server3.sin_family = AF_INET;
	server3.sin_port = htons(atoi(argv[1]));
	server3.sin_addr.s_addr = INADDR_ANY;

	server4.sin_family = AF_INET;
	server4.sin_port = htons(atoi(argv[2]));
	server4.sin_addr.s_addr = INADDR_ANY;

	if(bind(socket_desc1, (struct sockaddr *)&server1, sizeof(server1))<0)
	{
		printf("bind for socket1");
		exit(1);
	}


	if(bind(socket_desc2, (struct sockaddr *)&server2, sizeof(server2))<0)
	{
		printf("bind for socket2");
		exit(1);
	}
	
//	bzero((char *) &server3, sizeof(server3));

	if(bind(socket_desc3, (struct sockaddr *)&server3, sizeof(server3))<0)
	{
		printf("bind for socket3");
		exit(1);
	}
	

//	bzero((char *) &server4, sizeof(server4));

	if(bind(socket_desc4, (struct sockaddr *)&server4, sizeof(server4))<0)
	{
		printf("bind for socket4");
		exit(1);
	}



	if(listen(socket_desc3, 5)<0){
		printf("listening failes\n");
		exit(1);
	}

	if(listen(socket_desc4, 5)<0){
		printf("listening failes\n");
		exit(1);
	}

	printf("Listening to port %s & %s....\n",argv[1],argv[2]);

	n = sizeof(client);
	

	if(socket_desc1>socket_desc2)
	{
		maxfd = socket_desc1;
	}
	else maxfd = socket_desc2;

	if(maxfd<socket_desc3)
	{
		maxfd = socket_desc3;
	}
	if(maxfd<socket_desc4)
	{
		maxfd = socket_desc4;
	}

	while(1)
	{
		FD_ZERO(&read_set);
		FD_ZERO(&write_set);

		FD_SET(socket_desc1, &read_set);
		FD_SET(socket_desc2, &read_set);
		FD_SET(socket_desc3, &read_set);
		FD_SET(socket_desc4, &read_set);

		FD_SET(socket_desc1, &write_set);
		FD_SET(socket_desc2, &write_set);
		FD_SET(socket_desc3, &write_set);
		FD_SET(socket_desc4, &write_set);
		

		tval.tv_sec = 0;

		ret = select(maxfd+1, &read_set, &write_set, NULL, &tval);

		if(ret < 0)
		{
			printf("select");
			exit(1);
		}
		else if(FD_ISSET(socket_desc1, &read_set))
		{
			valread = recvfrom(socket_desc1, (char *)buffer, 1024, 0, (struct sockaddr *)&client, &n);
			if(valread < 0)
			{
				printf("error in recieving");
				exit(1);
			}
			buffer[valread] = '\0';
			printf("Received a UDP Stream in port %s:%s\n",argv[1],buffer);
		}
		else if(FD_ISSET(socket_desc2, &read_set))
		{
			valread = recvfrom(socket_desc2, (char *)buffer, 1024, 0, (struct sockaddr *)&client, &n);
			if(valread < 0)
			{
				printf("error in receiving");
				exit(1);
			}
			buffer[valread] = '\0';
			printf("Received a UDP Stream in port %s:%s\n",argv[2],buffer);
		}
		else if(FD_ISSET(socket_desc3, &read_set))
		{
			
			newsockfd1 = accept(socket_desc3, (struct sockaddr *) &client, (socklen_t *)&n);
			if(newsockfd1 < 0)
				error("Error on accept 3");
			valread = recv(newsockfd1, (char *)&buffer_len, sizeof(buffer_len), 0);
			if(valread<0)
			{
				printf("Error in recieving size at socket 3\n");
				exit(1);
			}
			valread = recv(newsockfd1, buffer, buffer_len, 0);
			if(valread<0)
			{
				printf("Error in recieving message at socket 3\n");
				exit(1);
			}
			buffer[buffer_len] = '\0';
			printf("Received a TCP Stream in port %s:%s\n",argv[1],buffer);
		}
		else if(FD_ISSET(socket_desc4, &read_set))
		{
			newsockfd2 = accept(socket_desc4, (struct sockaddr *) &client, (socklen_t *)&n);
			if(newsockfd2 < 0)
				error("Error on accept 4");
			valread = recv(newsockfd2, (char *)&buffer_len, sizeof(buffer_len), 0);
			if(valread<0)
			{
				printf("Error in recieving size at socket 4\n");
				exit(1);
			}
			valread = recv(newsockfd2, buffer, buffer_len, 0);
			if(valread<0)
			{
				printf("Error in recieving message at socket 4\n");
				exit(1);
			}
			buffer[buffer_len] = '\0';
			printf("Received a TCP Stream in port %s:%s\n",argv[2],buffer);
		}
	}
	close(socket_desc1);
	close(socket_desc2);
	close(socket_desc3);
	close(socket_desc4);
	return 0;

}
