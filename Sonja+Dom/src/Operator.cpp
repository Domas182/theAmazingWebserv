#include "Operator.hpp"
#include "Server.hpp"
#include "Poller.hpp"
#include "Client.hpp"


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

void Operator::start_process()
{
	for (size_t i = 0; i < _servers.size(); ++i)
	{
		if (_servers[i].setF_Content())
			throw std::invalid_argument("Error❗\nCould not open index file");
		if (_servers[i].setImg_Content())
			throw std::invalid_argument("Error❗\nCould not open image file");
		_servers[i].setResponse();
		_servers[i].setImg_Response();
		_servers[i].bindPort();
	}
	std::string termin;
	termin = "\r\n\r\n";

	PollFd poFD;

	std::vector<Client> clients;

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
					std::vector<unsigned char> request;
					request = _servers[clients[k].getIndex()].sockRecv(i, poFD);
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
						requestReady(request, clients[k], _servers[clients[k].getIndex()].getNBytes());
						// clients[k].setFlagT();
						if (clients[k].getFlag() == true)
						{
							std::cout << "1" << std::endl;
							if (clients[k].getCnt() == 0)
							{
								// clients[k].setResp(respone);
								clients[k].setResp(_servers[k].getResponse());
								clients[k].setCnt(1);
							} else if (clients[k].getCnt() == 1) {
								// clients[k].setResp(imgrespone);
								clients[k].setResp(_servers[k].getImg_Response());
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
							

							_servers[clients[k].getIndex()].sockSend(poFD.getPfd()[i].fd, clients[k]);
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
}
