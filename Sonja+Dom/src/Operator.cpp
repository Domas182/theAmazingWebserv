#include "Operator.hpp"
#include "Server.hpp"
#include "Poller.hpp"
#include "Client.hpp"
#include "../Request/RequestParser.hpp"


int fdServer(int fd, std::vector<Server> servers)
{
	for (size_t i = 0; i < servers.size(); i++)
	{
		if (fd == servers[i].getSockFd())
			return (i);
	}
	return (-1);
}

int lookClient(int fd, std::vector<Client> clients)
{
	for (size_t i = 0; i < clients.size(); i++)
	{
		if (fd == clients[i].getSock())
			return (i);
	}
	return (-1);
}

void requestReady(std::vector<unsigned char> request, Client& client, size_t bytes)
{
	size_t i = 0;
	if (client.getFlag() == false)
	{
//		while (i < bytes && request[i] != '\0' && client.getFlag() != true)

		while (i < bytes && request[i] != '\0' && client.getFlag() == false)
		{
			if ((request[i] == '\r' && request[i+1] == '\n' && request[i+2] == '\r' && request[i+3] == '\n') || (request[i] == '\r' && request[i+1] == '\n' && request[i+2] == '\n'))
			{
				client.printRequest();
				std::cout << "nbytes: " << bytes << "^^^^^^\n\n\n\n" << std::endl;
				client.setFlagT();
				break ;
			} else {
				client.pushRequest(request[i++]);
			}
		}
	}
}

void Operator::start_process()
{
	for (size_t i = 0; i < _servers.size(); ++i)
	{
		if (_servers[i].setF_Content())
			throw std::invalid_argument("Error❗\nCould not open index file");
		if (_servers[i].setImg_Content())
			throw std::invalid_argument("Error❗\nCould not open image file");
		if (_servers[i].setFavi_Content())
			throw std::invalid_argument("Error❗\nCould not open image file");
	
		_servers[i].setResponse();
		_servers[i].setImg_Response();
		_servers[i].setFavi_Response();
		_servers[i].bindPort();
	}
	std::string termin;
	termin = "\r\n\r\n";

	PollFd poFD;

	std::vector<Client> clients;
	//TODO:ASK SONJA 

	while(1)
	{
		poll(poFD.getPfd().data(), poFD.getFdCount(), 0); 
		if (poFD.getFdCount() == 0)
		{
			for (size_t j = 0; j < _servers.size(); j++)
			{
				_servers[j].continueSetup(_servers, j);
				poFD.addFd(_servers[j].getSockFd());
			}
		}
		std::vector<unsigned char> request;
		for (size_t i = 0; i < poFD.getFdCount(); i++)
		{
			if (poFD.getPfd()[i].revents & POLLIN) 
			{
				int k;
				if ((k = fdServer(poFD.getPfd()[i].fd, _servers)) != -1)
				{
					//new
					Client clie(k, _servers[k].sockAccept());
					poFD.addFd(clie.getSock());
					clients.push_back(clie);
				}
				else 
				{
					int k;
					k = lookClient(poFD.getPfd()[i].fd, clients);
					request = _servers[clients[k].getIndex()].sockRecv(i, poFD);
					// 	NOW THERES STUFF IN THE BUFFER TAHT HOLDS THE REQUEST
					if (request.size() <= 0)
					{
						std::vector<Client>::iterator it(clients.begin());
						for (int m = 0; m < k; m++)
							it++;
						clients.erase(it);
					}
					else
					{
						requestReady(request, clients[k], _servers[clients[k].getIndex()].getNBytes());
						if (clients[k].getFlag() == true)
						{
							RequestParser RP(clients[k].getRequest());

							if (RP.getURI() == "/favicon.ico")
								clients[k].setResp(_servers[clients[k].getIndex()].getFavi_Response());
							else if (RP.getURI() == "/cat.jpeg")				
								clients[k].setResp(_servers[clients[k].getIndex()].getImg_Response());
							else if (RP.getURI() == "/")
								clients[k].setResp(_servers[clients[k].getIndex()].getResponse());
							clients[k].getRequest().clear();
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
							

							_servers[clients[k].getIndex()].sockSend(poFD.getPfd()[i].fd, clients[k]);
							//std::cout << clients[k].getResponseSize() << std::endl;
							clients[k].clearResponse();
							//if (clients[k].getResponseSize() == 0)
							//{
							//	//somehow handle error status codes
							//	if (clients[k].getStatusCode() == "420")
							//	{
							//		close(poFD.getPfd()[i].fd);
							//		poFD.deleteFd(i);
							//	}
							//	if (clients[k].getStatusCode() == "69")
							//		sleep(10);
							//	clients[k].clearResponse();
							//}
						}
					}
				}
			}
		}
	}
}
