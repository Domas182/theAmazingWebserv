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

int	g_error;

int fdServer(int fd, std::vector<Server>& servers)
{
	for (size_t i = 0; i < servers.size(); i++)
	{
		if (fd == servers[i].getSockFd())
			return (i);
	}
	return (-1);
}

int lookClient(int fd, std::vector<Client>& clients)
{
	for (size_t i = 0; i < clients.size(); i++)
	{
		if (fd == clients[i].getSock())
			return (i);
	}
	return (-1);
}

int	findBodyLength(std::vector<unsigned char>& request)
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

bool crlfBool(std::vector<unsigned char>& data, size_t i)
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

void chunkedHandler(Client& client, std::vector<unsigned char>& request, size_t& i, size_t bytes)
{
	while (i < bytes)
	{
		if (!client.chunkSizeSet)
		{
			while (i < bytes && !crlfBool(request, i))
				client.iHex.push_back(request[i++]);
			if (request[i] == '\r')
			{
				while (i < bytes && crlfBool(request, i))
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
				while (i < bytes && crlfBool(request, i))
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

void headerCountAndFlags(Client& client, int len)
{
	client.setFlagT();
	len = findBodyLength(client.tmpReq);
	headerFlagSetter(client, len);
	client.tmpLen = len;
}

void	RequestChecker(std::vector<unsigned char>& request, Client& client, Server& server, size_t bytes)
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
	if ( client.tmpLen > server.getLimitBody())
		client.setRFlagT();
	if (client.getHBFlag() && client.getFlag() && !client.getRFlag())
	{
		if (!client.getRFlag() && !client.getCFlag())
		{
			//hier vlt check für den client.statcode?, DAMIT DASS dann gleich weiter geht?
			
			while (i < bytes)
				client.tmpBody.push_back(request[i++]);
			if (client.tmpBody.size() == client.tmpLen)
			{
				client.setRFlagT();
				client.tmpLen = 0;
			}
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

void setupServers(std::vector<Server>& servs, PollFd& poFD)
{
	for (size_t i = 0; i < servs.size(); i++)
	{
		servs[i].continueSetup(servs, i);
		poFD.addFd(servs[i].getSockFd());
	}
}

void	Operator::dataOnServer(std::vector<Client>& clients, PollFd& poFD, int i)
{
	Client clie(i, _servers[i].sockAccept());
	poFD.addFd(clie.getSock());
	clients.push_back(clie);
}


void	Operator::dataOnClient(std::vector<Client>& clients, PollFd& poFD, int i)
{
	int cIndex = lookClient(poFD.getPfd()[i].fd, clients);
	std::vector<unsigned char> request;
	request = _servers[clients[cIndex].getIndex()].sockRecv(i, poFD);
	if (request.size() <= 0)
	{
		std::vector<Client>::iterator it(clients.begin());
		for (int m = 0; m < cIndex; m++)
			it++;
		clients.erase(it);		
	} else {
		try	{
			RequestChecker(request, clients[cIndex], _servers[clients[cIndex].getIndex()], _servers[clients[cIndex].getIndex()].getNBytes());
			}catch(const std::exception& e)	{
				Response tmpRSP;
				clients[cIndex].setResp(tmpRSP.createErrorResponse(g_error, _servers[clients[cIndex].getIndex()]));
				if (g_error != 200)
					g_error = 200;
				clients[cIndex].clearRequest();
			}
			if (clients[cIndex].getRFlag())
			{
				clients[cIndex].printRequest();

				RequestParser RP(clients[cIndex].tmpReq);
				clients[cIndex].printBody();
				int sIndex;
				sIndex = find_server(RP.getPort());
				Handler H(RP, clients[cIndex]);
				H.start_handling(_servers[sIndex], clients[cIndex]);
			}
			request.clear();
		}
}

void	Operator::dataToSend(std::vector<Client>& clients, PollFd& poFD, int i)
{
	int cIndex;
	if ((cIndex = lookClient(poFD.getPfd()[i].fd, clients)) != -1)
	{
		if (clients[cIndex].getResponseSize() > 0)
		{
			_servers[clients[cIndex].getIndex()].sockSend(poFD.getPfd()[i].fd, clients[cIndex]);
			if (clients[cIndex].getResponseSize() == 0)
			{
				if (clients[cIndex].getStatusCode() == "413"){
					close(poFD.getPfd()[i].fd);
					poFD.deleteFd(i);
				}
				clients[cIndex].resetClient();
			}							
		}
	}	
}

void Operator::start_process()
{
	g_error = 200;
	PollFd poFD;
	std::vector<Client> clients;

	for (size_t i = 0; i < _servers.size(); ++i)
		_servers[i].bindPort();
	while(1)
	{
		poll(poFD.getPfd().data(), poFD.getFdCount(), 0); 
		if (!poFD.getFdCount())
			setupServers(_servers, poFD);
		for (size_t i = 0; i < poFD.getFdCount(); i++)
		{
			if (poFD.getPfd()[i].revents & POLLIN)
			{
				int k;
				if ((k = fdServer(poFD.getPfd()[i].fd, _servers)) != -1)
					dataOnServer(clients, poFD, k);
				else
					dataOnClient(clients, poFD, i);
			}
			if (i < poFD.getFdCount())
				if (poFD.getPfd()[i].revents & POLLOUT)
					dataToSend(clients, poFD, i);
		}
	}
}
