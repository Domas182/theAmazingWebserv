#include <sys/socket.h>
#include <netinet/if_ether.h>
#include <cstdlib>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <poll.h>
#include <fstream>
#include <sstream>
#include <sys/wait.h>
#define PORT 9999

std::string readFileIntoString2(const std::string& path) {
    std::ostringstream ss;
	std::ifstream input_file(path);
    if (!input_file.is_open()) {
        std::cerr << "Could not open the file - '"
             << path << "'" << std::endl;
        exit(EXIT_FAILURE);
    }
    ss << input_file.rdbuf();
    return ss.str();
}

int	main(void)
{

	std::string filename("test.html");
	std::string f_content;
	f_content = readFileIntoString2(filename);

	std::string	imgfilename("cat.jpeg");
	std::string img_content;
	img_content = readFileIntoString2(imgfilename);



	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	fcntl(sockfd, F_SETFL, O_NONBLOCK);	

	if (sockfd == -1)
	{
		std::cout << "Socket creation failed because of error: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

	const int trueFlag = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &trueFlag, sizeof(int)) < 0)
		perror("Failure");


	struct pollfd mypoll[2];
	memset(&mypoll[0], 0, sizeof(mypoll[0]));
	memset(&mypoll[1], 0, sizeof(mypoll[1]));
	mypoll[0].events = POLLIN;

	sockaddr_in	sockaddr[2];
	sockaddr[0].sin_family = AF_INET;
	sockaddr[0].sin_addr.s_addr = INADDR_ANY;
	sockaddr[0].sin_port = htons(PORT);

	
	if (bind(sockfd, (struct sockaddr*)&sockaddr[0], sizeof(sockaddr[0])) < 0)
	{
		std::cout << "Failed to bind to port 9999. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

	
	if (listen(sockfd, 10) < 0)
	{
		std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

	unsigned long addrlen = sizeof(sockaddr[0]);

	char buffer[1000];
	int cnt = 0;

	int connection, connection1;

// 	while (1)
// 	{
// 		connection = accept(sockfd, (struct sockaddr*)&sockaddr[0], (socklen_t*)&addrlen);	
// 		connection1 = accept(sockfd1, (struct sockaddr*)&sockaddr[1], (socklen_t*)&addrlen);

// 		if (connection >= 0 || connection1 >= 0)
// 		{
// 			while (1)
// 			{
// 				mypoll[0].fd = connection;
// 				mypoll[1].fd = connection1;
// //				if (poll(mypoll, 2, 100) == 1)
// 				if (poll(&mypoll[0], 1, 1000) == 1)
// 				{
// 					long bytesRead = read(connection, buffer, 1000);
// 					if (strncmp(buffer, "done", 4) == 0)
// 					{
// 						//
// 						std::string respone = "HTTP/1.1 200 OK";
// 						send(connection, respone.c_str(), respone.size(), 0);
// 						//
// 						close(connection);
// 						std::cout << "connection closed\n";
// 						cnt = 0;
// 						break;
// 					}
// 					std::cout << buffer << std::endl;
// 				//
// 					std::string respone;
// 					std::string imgrespone;
// 					respone = "HTTP/1.1 200 OK\nDate: Thu. 20 May 2004 21:12:58 GMT\nConnection: keep-alive\nServer: Apache/1.3.27\nContent-Type: text/html\n\r\n";
// 					imgrespone = "HTTP/1.1 200 OK\nDate: Thu. 20 May 2004 21:12:58 GMT\nConnection: close\nServer: Apache/1.3.27\nContent-Type: image/jpeg\n\r\n";

// 					//append html file????
// //					std::cout << f_content.size() << std::endl;
// 					respone.append(f_content);
// 					imgrespone.append(img_content);
// 					// for (int i = 0; i < 7; i++)
// 					// 	send(connection, respone[i].c_str(), respone[i].length(), 0);
// 					memset(buffer, 0, sizeof(buffer));
// 					send(connection, respone.c_str(), respone.size(), 0);
// 					std::cout << "second" << buffer << std::endl;
// 					// while (poll(&mypoll[0], 1, 100) != 1)
// 					// 	;


// 					//usleep(200);
// 					//close(connection);
// 					//connection = accept(sockfd, (struct sockaddr*)&sockaddr[0], (socklen_t*)&addrlen);	
// 					//mypoll[0].fd = connection;
					
// 					//bytesRead = read(connection, buffer, 1000);
// 					std::cout << "third" << buffer << std::endl;


// 					//send(connection, imgrespone.c_str(), imgrespone.size(), 0);
// 					//close(connection);
// 					//exit(0);
// 				//
// 					// std::cout << "The message was: " << buffer << "and it took you " << cnt * 100 << "ms to print it.\n" << std::endl;
// 					// memset(buffer, 0, sizeof(buffer));	
// 					// cnt = 0;
// 				} else if (poll(&mypoll[1], 1, 100) == 1){
// 					long bytesRead = read(connection1, buffer, 100);
// 					if (strncmp(buffer, "done", 4) == 0)
// 					{
// 						close(connection1);
// 						std::cout << "connection closed\n";
// 						cnt = 0;
// 						break;
// 					}
// 					std::cout << "The message was: '" << buffer << "' and it took you " << cnt * 100 << "ms to print it.\n" << std::endl;
// 					memset(buffer, 0, sizeof(buffer));
// 					cnt = 0;
// 				}
// 				cnt++;
// 			}
// 		}	
// 	}


	std::string respone;
	std::string imgrespone;
	respone = "HTTP/1.1 200 OK\nDate: Thu. 20 May 2004 21:12:58 GMT\nConnection: close\nServer: Apache/1.3.27\nContent-Type: text/html\n\r\n";
	imgrespone = "HTTP/1.1 200 OK\nDate: Thu. 20 May 2004 21:12:58 GMT\nConnection: close\nServer: Apache/1.3.27\nContent-Type: image/jpeg\n\r\n";

	respone.append(f_content);
	imgrespone.append(img_content);

	std::string termin;
	const char[5] 
	termin = "\r\n\r\n";

	while (1)
	{
		connection = accept(sockfd, (struct sockaddr*)&sockaddr[0], (socklen_t*)&addrlen);	

		mypoll[0].fd = connection;

		//std::cout << poll(&mypoll[0], 1, 100) << "\n";
		if (poll(&mypoll[0], 1, 1000) == 1)
		{
			long bytesRead = read(connection, buffer, 1000);
			std::cout << buffer << std::endl;
			if (cnt == 0)
				send(connection, respone.c_str(), respone.length(), 0);
			else
				send(connection, imgrespone.c_str(), imgrespone.length(), 0);
			memset(buffer, 0, sizeof(buffer));
			cnt++;

			std::cout << cnt << std::endl;
			if (cnt == 2)
			{
				send(connection, termin.c_str(), termin.size(), 0);
				cnt = 0;
				close(connection);
			}
		}
		std::cout << "cnt: " << cnt << std::endl;
	}	


	std::string response = "Good talking to you.\n";
	send(connection, response.c_str(), response.size(), 0);

	close(connection);
	close(sockfd);

	return 0;
}
