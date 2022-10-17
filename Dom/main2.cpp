#include <sys/socket.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <cstdlib>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <poll.h>
#include <fstream>
#include <sstream>
#include <sys/wait.h>
#include <vector>
#include "socket.hpp"
#include "server.hpp"
#include "client.hpp"

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

int		lookClient(int fd, std::vector<Client> clients)
{
	for (size_t i = 0; i < clients.size(); i++)
	{
		if (fd == clients[i].getSock())
			return (i);
	}
	return (-1);
}

int		fdServer(int fd, std::vector<Server> servers)
{
	for (size_t i = 0; i < servers.size(); i++)
	{
		if (fd == servers[i].getSockFd())
			return (i);
	}
	return (-1);
}

void requestReady(std::vector<unsigned char> request, Client& client, size_t bytes)
{
	size_t i = 0;
	if (client.getFlag() == false)
	{
		while (i < bytes && request[i] != '\0' && client.getFlag() != true)
		{
			if (request[i] == '\r' && request[i+1] == '\n' && request[i+2] == '\r' && request[i+3] == '\n')
			{
				client.setFlagT();
			}
			i++;
		}
	}
}

int main(void)
{
	std::string filename("test.html");
	std::string f_content;
	f_content = readFileIntoString2(filename);

	std::string	imgfilename("cat.jpeg");
	std::string img_content;
	img_content = readFileIntoString2(imgfilename);
	std::string respone;
	std::string imgrespone;
	std::cout << f_content.size() << std::endl;
	respone = "HTTP/1.1 200 OK\nDate: Thu. 20 May 2004 21:12:58 GMT\nConnection: close\nServer: Apache/1.3.27\nContent-Type: text/html\nContent-Length: 151\n\r\n";
	std::cout << img_content.size() << std::endl;
	imgrespone = "HTTP/1.1 200 OK\nDate: Thu. 20 May 2004 21:12:58 GMT\nConnection: close\nServer: Apache/1.3.27\nContent-Type: image/jpeg\nContent-Length: 29036\n\r\n";

	respone.append(f_content);
	imgrespone.append(img_content);

	std::string termin;
	termin = "\r\n\r\n";

    PollFd poFD;
    //std::vector<struct client> clients;

	//new
	std::vector<Server> servers;
	std::vector<Client> clients;


	//temp server creation
	for (int x = 0; x < 3; x++)
	{
		Server serv(9999 - x);
		servers.push_back(serv);
	}

	while(1)
	{

        poll(poFD.getPfd().data(), poFD.getFdCount(), 0); 
        if (poFD.getFdCount() == 0)
        {
			for (size_t j = 0; j < servers.size(); j++)
			{
				servers[j].continueSetup(servers, j);
				poFD.addFd(servers[j].getSockFd());
			}
        }
        for (size_t i = 0; i < poFD.getFdCount(); i++)
        {
            if (poFD.getPfd()[i].revents & POLLIN) 
            {
                int k;
                if ((k = fdServer(poFD.getPfd()[i].fd, servers)) != -1)
                {
					//new
					Client clie(k, servers[k].sockAccept());
					poFD.addFd(clie.getSock());
					clients.push_back(clie);
                }
                else 
                {
                    int k;
                    k = lookClient(poFD.getPfd()[i].fd, clients);
                    std::vector<unsigned char> request;
                    request = servers[clients[k].getIndex()].sockRecv(i, poFD);
                    if (request.size() <= 0)
                    {
                        std::vector<Client>::iterator it(clients.begin());
                        for (int m = 0; m < k; m++)
                            it++;
                        clients.erase(it);
                    }
                    else
                    {
						//check if header is complete
						requestReady(request, clients[k], servers[clients[k].getIndex()].getNBytes());
						// clients[k].setFlagT();
						if (clients[k].getFlag() == true)
						{
							std::cout << "1" << std::endl;
							if (clients[k].getCnt() == 0)
							{
								clients[k].setResp(respone);
								clients[k].setCnt(1);
							} else if (clients[k].getCnt() == 1) {
								clients[k].setResp(imgrespone); 
								clients[k].setCnt(0);
							}
							clients[k].setFlagF();
						}
                    }
                }
            }
            if (i < poFD.getFdCount())
            {
                if (poFD.getPfd()[i].revents & POLLOUT)
                {
                    int k;
                    if ((k = lookClient(poFD.getPfd()[i].fd, clients)) != -1)
                    {
                        if (clients[k].getResponseSize() >= 0)
                        {
							//new
							

							servers[clients[k].getIndex()].sockSend(poFD.getPfd()[i].fd, clients[k]);
							//std::cout << clients[k].response.size() << std::endl;
							if (clients[k].getResponseSize() == 0)
                            {
								//somehow handle error status codes
                                if (clients[k].getStatusCode() == "420")
                                {
                                    close(poFD.getPfd()[i].fd);
                                    poFD.deleteFd(i);
                                }
                                if (clients[k].getStatusCode() == "69")
                                    sleep(10);
                                clients[k].clearResponse();
                             }
                        }
                    }
                }
            }
        }
	}

    return 0;
}
