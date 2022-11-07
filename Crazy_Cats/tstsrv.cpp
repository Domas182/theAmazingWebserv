#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <poll.h>
#define PORT 9999
#define PORT1 9998

//#include <typeinfo>

int	main(void)
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);	
	int sockfd1 = socket(AF_INET, SOCK_STREAM, 0);	
	if (sockfd == -1)
	{
		std::cout << "Socket creation failed because of error: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
	if (sockfd1 == -1)
	{
		std::cout << "Socket creation failed because of error: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

	const int trueFlag = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &trueFlag, sizeof(int)) < 0)
		perror("Failure");
	if (setsockopt(sockfd1, SOL_SOCKET, SO_REUSEADDR, &trueFlag, sizeof(int)) < 0)
		perror("Failure");

	struct pollfd mypoll[2];
	memset(&mypoll[0], 0, sizeof(mypoll[0]));
	memset(&mypoll[1], 0, sizeof(mypoll[1]));
	mypoll[0].events = POLLIN;
	mypoll[1].events = POLLIN;

	sockaddr_in	sockaddr[2];
	sockaddr[0].sin_family = AF_INET;
	sockaddr[0].sin_addr.s_addr = INADDR_ANY;
	sockaddr[0].sin_port = htons(PORT);
	sockaddr[1].sin_family = AF_INET;
	sockaddr[1].sin_addr.s_addr = INADDR_ANY;
	sockaddr[1].sin_port = htons(PORT1);
	
	if (bind(sockfd, (struct sockaddr*)&sockaddr[0], sizeof(sockaddr[0])) < 0)
	{
		std::cout << "Failed to bind to port 9999. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
	if (bind(sockfd1, (struct sockaddr*)&sockaddr[1], sizeof(sockaddr[1])) < 0)
	{
		std::cout << "Failed to bind to port 9999. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
	
	if (listen(sockfd, 10) < 0)
	{
		std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
	if (listen(sockfd1, 10) < 0)
	{
		std::cout << "Failed to listen on socket1. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
	
	unsigned long addrlen = sizeof(sockaddr[0]);


	int connection = accept(sockfd, (struct sockaddr*)&sockaddr[0], (socklen_t*)&addrlen);
	int connection1 = accept(sockfd1, (struct sockaddr*)&sockaddr[1], (socklen_t*)&addrlen);
	if (connection < 0 || connection1 < 0)
	{
		std::cout << "Failed to grab connection. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}	

	char buffer[100];
	int cnt = 0;

	mypoll[0].fd = connection;
	mypoll[1].fd = connection1;

	while (1)
	{
		if (poll(&mypoll[0], 1, 100) == 1)
		{
			long bytesRead = read(connection, buffer, 100);
			if (strncmp(buffer, "done", 4) == 0)
				break;
			std::cout << "The message was: " << buffer << "and it took you " << cnt * 100 << "ms to print it.\n" << std::endl;
			memset(buffer, 0, sizeof(buffer));	
			cnt = 0;
		} else if (poll(&mypoll[1], 1, 100) == 1){
			long bytesRead = read(connection1, buffer, 100);
			if (strncmp(buffer, "done", 4) == 0)
				break;
			std::cout << "The message was: '" << buffer << "' and it took you " << cnt * 100 << "ms to print it.\n" << std::endl;
			memset(buffer, 0, sizeof(buffer));	
			cnt = 0;
		}else{
				cnt++;		
		}
	}
	std::string response = "Good talking to you.\n";
	send(connection, response.c_str(), response.size(), 0);
	send(connection1, response.c_str(), response.size(), 0);

	close(connection);
	close(connection1);
	close(sockfd);
	close(sockfd1);

	return 0;
}
