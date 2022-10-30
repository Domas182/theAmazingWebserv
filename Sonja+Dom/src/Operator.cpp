#include "Operator.hpp"
#include "Server.hpp"
#include "Poller.hpp"
#include "Client.hpp"
#include "stdlib.h"
#include "../Request/RequestParser.hpp"
#include "../Handler/Handler.hpp"
#include "../Response/Response.hpp"


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

size_t	findBodyLength(Client& client)
{
	if (client.getFlag() == true)	
	{
		RequestParser RP(client.getRequest());
		std::unordered_map<std::string, std::string>::const_iterator got = RP.getRequestH().find("Content-Length");
		std::unordered_map<std::string, std::string>::const_iterator endit = RP.getRequestH().end();
		if (got != endit)
		{
			size_t contLen = atoi(got->second.c_str());
			return (contLen);
		}
		std::unordered_map<std::string, std::string>::const_iterator ITE = RP.getRequestH().find("Transfer-Encoding");
		if (ITE != endit)
		{
			client.setCFlagT();
			// retmakurn (-1);
		// if (RP.getRequestH().find("Transfer-Encoding")->second == "chunked")
		}
	}
	return (0);
}

void extractBody(std::vector<unsigned char>& request, int index, size_t contLen, Client & client)
{
	for (size_t i = 0; i < contLen; i++)
	{
		client.pushBody(request[index + i]);
		//client.RP.pushBody()
	}
	if (client.getBodySize() == contLen)
		client.setBFlagT();
}

// void extractChunkedBody()

int getRequestReady(std::vector<unsigned char>& request, Client& client, size_t bytes)
{
		size_t i = 0;
		for (; i < bytes; i++)
		{
			if (request[i] == '\r' && request[i+1] == '\n' && request[i+2] == '\r' && request[i+3] == '\n')
			{
				i += 4;
				for (int k = 0; k < 2; k++)
				{
					client.pushRequest('\r');
					client.pushRequest('\n');
				}	
				client.setFlagT();
				size_t content = findBodyLength(client);
				if (client.getCFlag() == true)
				{
					std::cout << PINK << "CHUNKED!!!" << RESET << std::endl;
					// extractChunkedBody()
				}
				if (content > 0)
					extractBody(request, i, content, client);
				break;
			}
			client.pushRequest(request[i]);
		}
		return (i);
}

int	Operator::find_server(uint32_t port)
{
	int i = 0;
	for (std::vector<Server>::const_iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		if (it->getPort() == port)
			return (i);
		i++;
	}
	return (i);
}

void Operator::start_process()
{
	for (size_t i = 0; i < _servers.size(); ++i)
	{
		_servers[i].bindPort();
	}
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
					if (request.size() <= 0)
					{
						std::vector<Client>::iterator it(clients.begin());
						for (int m = 0; m < k; m++)
							it++;
						clients.erase(it);
					}
					else
					{
						getRequestReady(request, clients[k], _servers[clients[k].getIndex()].getNBytes());
						if (clients[k].getFlag() == true)
						{
							RequestParser RP(clients[k].getRequest());
							int i = find_server(RP.getPort());
							Handler H(RP, clients[k]);
							H.start_handling(_servers[i]);
							clients[k].setResp(_servers[i].getResponse());
							//TODO: if this is in the Handler, we can pack all the previous functions in the handler too :)
							clients[k].setBFlagF();
							clients[k].setFlagF();
							request.clear();
							clients[k].clearRequest();
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
							_servers[clients[k].getIndex()].sockSend(poFD.getPfd()[i].fd, clients[k]);
							clients[k].clearResponse();
						}
					}
				}
			}
		}
	}
}
