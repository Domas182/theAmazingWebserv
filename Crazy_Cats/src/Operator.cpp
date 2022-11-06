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

bool crlfBool(std::vector<unsigned char> data, size_t i)
{
	if (data[i] == '\r' && data[i + 1] == '\n')
		return true;
	else
		return false;
}

void crlfPush(Client& client)
{
	client.tmpReq.push_back('\r');
	client.tmpReq.push_back('\n');	
}

void chunkedHandler(Client& client, std::vector<unsigned char> request, size_t& i, size_t& bytes)
{
	while (i < bytes)
	{
		if (!client.chunkSizeSet)
		{
			while (!crlfBool(request, i) && i < bytes)
				client.iHex.push_back(request[i++]);
			if (request[i] == '\r')
			{
				while (crlfBool(request, i) && i < bytes)
					i += 2;
				client.chunkSize = std::stoi(client.iHex.c_str(), nullptr, 16);
				client.chunkSizeSet = true;
				client.iHex.clear();
			}
		}
		if (client.chunkSizeSet)
		{
			if (client.chunkSize == 0){
				client.setRFlagT();
			} else {
				while (client.tmpChunkedBody.size() < client.chunkSize && i < bytes)
					client.tmpChunkedBody.push_back(request[i++]);
				if (client.tmpChunkedBody.size() == client.chunkSize)
				{
					for (size_t x = 0; x < client.tmpChunkedBody.size(); x++)
						client.tmpBody.push_back(client.tmpChunkedBody[x]);
					client.chunkSizeSet = false;
					client.tmpChunkedBody.clear();
				}
				while (crlfBool(request, i) && i < bytes)
					i += 2;
			}
		}
		// pass appropriate response codes
	}	
}

void headerFlagSetter(Client& client, int len)
{
	if (len > 0){
		client.setFlagT();
		client.setHBFlagT();
	}else if (len == -1 ){
		client.setFlagT();
		client.setCFlagT();
		client.setHBFlagT();
		client.chunkSizeSet = false;	
	}else{
		client.setFlagT();
		client.setRFlagT();		
	}
}

void headerCountAndFlags(Client& client, int& len)
{
	client.setFlagT();
	len = findBodyLength(client.tmpReq);
	headerFlagSetter(client, len);
}

void	RequestChecker(std::vector<unsigned char> request, Client& client, Server& server, size_t& bytes)
{
	size_t i = 0;
	int contLen = 0;
	if (!client.getFlag())
	{
		while(!client.getFlag() && request[i] != '\0' && i < bytes)
		{
			if(crlfBool(request, i) || request[i] == '\n')
			{
				if (request[i] == '\r')
				{
					i += 2;
					crlfPush(client);
				}
				else
					i++;
				if ((client.tmpReq.size() == 1 && request[0] == '\n') || client.tmpReq.size() == 0)	{
					headerCountAndFlags(client, contLen);
				} else if (crlfBool(request, i))
				{
					i += 2;
					for (size_t j = 0; j < 2; j++)
						crlfPush(client);
				headerCountAndFlags(client, contLen);
				}
			} else {
				client.tmpReq.push_back(request[i++]);
			}
		}
	}
	if (client.getHBFlag() && client.getFlag() && !client.getRFlag())
	{
		if (!client.getRFlag() && !client.getCFlag())
		{
			while (client.tmpBody.size() < contLen && i < bytes)
				client.tmpBody.push_back(request[i++]);
			if (client.tmpBody.size() == contLen)
				client.setRFlagT();
		}
		if (!client.getRFlag() && client.getCFlag())
			chunkedHandler(client, request, i, bytes);
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
					// if (request.size() > 1)
					// {
					// 	for (int i = 0; i < request.size(); i++)
					// 		std::cout << request[i];
					// }
					std::cout << "read loop get's here. size of tmpbody: " << clients[k].tmpBody.size() << std::endl;
					
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
							// for (size_t i = 0; i < clients[k].tmpBody.size(); i++)
							// 	std::cout << clients[k].tmpBody[i];
							std::cout << clients[k].tmpBody.size() << std::endl;
							std::cout << "do you get here?\n";
							//RequestParser RP(clients[k].getRequest());
							RequestParser RP(clients[k].tmpReq);
							int i = find_server(RP.getPort());
							Handler H(RP, clients[k]);
              				H.start_handling(_servers[i], clients[k]);
							// clients[k].setResp(_servers[i].getResponse());
							if (clients[k].getHBFlag())
							{
								write2file(clients[k].tmpBody, "test.txt");
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

						///random
						request.clear();
						///

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
