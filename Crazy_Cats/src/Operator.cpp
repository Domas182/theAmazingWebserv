#include "Operator.hpp"
#include "Server.hpp"
#include "Poller.hpp"
#include "Client.hpp"
#include "stdlib.h"
#include "../Request/RequestParser.hpp"
#include "../Handler/Handler.hpp"
#include "../Response/Response.hpp"
#include <fstream>
#include <iostream>


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

// size_t	findBodyLength(Client& client)
// {
// 	if (client.getFlag() == true)	
// 	{
// 		RequestParser RP(client.getRequest());
// 		std::unordered_map<std::string, std::string>::const_iterator got = RP.getRequestH().find("Content-Length");
// 		std::unordered_map<std::string, std::string>::const_iterator endit = RP.getRequestH().end();
// 		if (got != endit)
// 		{
// 			size_t contLen = atoi(got->second.c_str());
// 			return (contLen);
// 		}
// 		std::unordered_map<std::string, std::string>::const_iterator ITE = RP.getRequestH().find("Transfer-Encoding");
// 		if (ITE != endit)
// 		{
// 			return (-1);
// 			//macro like transfer encoding for that
// 		}
// 	}
// 	return (0);
// }

int	findBodyLength(std::vector<unsigned char> request)
{

	RequestParser RP(request);
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
		return (-1);
		//macro like transfer encoding for that
	}
	return (0);
}


void extractBody(std::vector<unsigned char>& request, int index, size_t contLen, Client & client)
{
	for (size_t i = 0; (i < contLen) && request[index + i]; i++)
	{
		std::cout << "ContLen: " << contLen << " i: " << i << std::endl;
		client.pushBody(request[index + i]);
		//client.RP.pushBody()
	}
		std::cout << "DO YOU GETtyherere HERE?" << std::endl;
	if (client.getBodySize() == contLen)
		client.setBFlagT();
}

//new attempt
void	RequestChecker(std::vector<unsigned char> request, Client& client, Server& server, size_t& bytes)
{
	size_t i = 0;
	int contLen = 0;

	if (!client.getFlag())
	{
		//while(!client.getFlag() && request[i] != '\0' && i < bytes)
		while(!client.getFlag() && request[i] != '\0' && i < bytes)
		{
			if(request[i] == '\r' && request[i + 1] == '\n' && request[i + 2] == '\r' && request[i + 3] == '\n')
			{
				i += 4;
				client.setFlagT();
				for (int x = 0; x < 2; x++)
				{
					client.tmpReq.push_back('\r');
					client.tmpReq.push_back('\n');
				}
				contLen = findBodyLength(client.tmpReq);
				std::cout << " CONT LEN!!! ::: " << contLen << std::endl;

				if (contLen > 0) {
					client.setFlagT();
					client.setHBFlagT();
				}
				else if (contLen == -1)	{
					client.setFlagT();
					client.setCFlagT();
				} else	{
					client.setFlagT();
					client.setRFlagT();
				}
			} else {
				client.tmpReq.push_back(request[i++]);
			}
		}
	}
	if (client.getHBFlag() && client.getFlag() && !client.getRFlag() && !client.getBFlag() && !client.getCFlag())
	{
		if (contLen > server.getLimitBody())
		{
			std::cout << contLen << std::endl;
			throw std::out_of_range("Error! \n Body size is higher than MAX_BODY.");
		}
		if (!client.getRFlag())
		{
			while (client.tmpBody.size() < contLen)
			{
				client.tmpBody.push_back(request[i]);
				i++;
			}
			if (client.tmpBody.size() == contLen)
			{
				std::cout << "yes\n";
				client.setBFlagT();
				client.setRFlagT();
			}
		}
	} else if (client.getFlag() && client.getCFlag()){
		while (request[i])
		{
			client.tmpBody.push_back(request[i]);
			i++;
		}
		for (size_t x = 0; x < client.tmpBody.size(); x++)
			std::cout << client.tmpBody[x];
	}
}

void bodyExtractor(Client& client)
{
	size_t i = 0;
	while(client.tmpBody[i] != '\r' && client.tmpBody[i+1] != '\n' && client.tmpBody[i+2] != '\r' && client.tmpBody[i+3] != '\n')
	{
		i++;
	}
	i +=4;
	while(client.tmpBody[i] != '\r' && client.tmpBody[i+1] != '\n')
	{
		client.tmpExtract.push_back(client.tmpBody[i]);
		i++;
	}
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

void write2file(std::vector<unsigned char> input, std::string filename)
{
	std::fstream file;
	file.open(filename, std::ios_base::out);
	for (size_t i = 0; i < input.size(); i++)
		file << input[i];
	file.close();
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
					//request = _servers[clients[k].getIndex()].testRecv(i, poFD);

					
					if (request.size() <= 0)
					{
						std::vector<Client>::iterator it(clients.begin());
						for (int m = 0; m < k; m++)
							it++;
						clients.erase(it);
					}
					else
					{
						//getRequestReady(request, clients[k], _servers[clients[k].getIndex()].getNBytes());
						RequestChecker(request, clients[k], _servers[clients[k].getIndex()], _servers[clients[k].getIndex()].getNBytes());
						if (clients[k].getRFlag())
						{
							clients[k].printRequest();
							for (size_t i = 0; i < clients[k].tmpBody.size(); i++)
								std::cout << clients[k].tmpBody[i];
							std::cout << "do you get here?\n";
							//RequestParser RP(clients[k].getRequest());
							RequestParser RP(clients[k].tmpReq);
							int i = find_server(RP.getPort());
							Handler H(RP, clients[k]);
              				H.start_handling(_servers[i], clients[k]);
							// clients[k].setResp(_servers[i].getResponse());
							if (clients[k].getHBFlag())
							{
								//bodyExtractor(clients[k]);
								write2file(clients[k].tmpBody, "test.png");
							}
							//TODO: if this is in the Handler, we can pack all the previous functions in the handler too :)
							clients[k].setBFlagF();
							clients[k].setFlagF();
							clients[k].setHBFlagF();
							clients[k].setRFlagF();
							clients[k].setCFlagF();
							clients[k].tmpReq.clear();
							clients[k].tmpBody.clear();
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
