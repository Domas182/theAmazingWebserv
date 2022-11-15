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

void 	Operator::RequestChecker(std::vector<unsigned char>& request, int c)
{
	size_t i = 0;
	int contLen = 0;
	if (!_clients[c].getFlag())
	{
		while(!_clients[c].getFlag() && request[i] != '\0' && i < _servers[_clients[c].getIndex()].getNBytes())
		{
			if(crlfBool(request, i) || request[i] == '\n')
			{
				if (request[i] == '\r')
				{
					i += 2;
					_clients[c].crlfPush();
				}
				else
					i++;
				if ((_clients[c].tmpReq.size() == 1 && request[0] == '\n') || _clients[c].tmpReq.size() == 0)	{
					_clients[c].headerCountAndFlags(contLen);
				} else if (crlfBool(request, i))
				{
					i += 2;
					for (size_t j = 0; j < 2; j++)
						_clients[c].crlfPush();
					_clients[c].headerCountAndFlags(contLen);
				}
			} else {
				_clients[c].tmpReq.push_back(request[i++]);
			}
		}
	}
	if (_clients[c].tmpLen > _servers[c].getLimitBody())
		_clients[c].setRFlagT();
	if (_clients[c].getHBFlag() && _clients[c].getFlag() && !_clients[c].getRFlag())
	{
		if (!_clients[c].getRFlag() && !_clients[c].getCFlag())
		{
			while (i < _servers[_clients[c].getIndex()].getNBytes())
				_clients[c].tmpBody.push_back(request[i++]);
			if (_clients[c].tmpBody.size() == _clients[c].tmpLen)
			{
				_clients[c].setRFlagT();
				_clients[c].tmpLen = 0;
			}
		}
		if (!_clients[c].getRFlag() && _clients[c].getCFlag())
			_clients[c].chunkedHandler(request, i, _servers[_clients[c].getIndex()].getNBytes());
	}
}

int Operator::fdServer(int fd)
{
	for (size_t i = 0; i < _servers.size(); i++)
		if (fd == _servers[i].getSockFd())
			return (i);
	return (-1);
}

int Operator::lookClient(int fd)
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (fd == _clients[i].getSock())
			return (i);
	}
	return (-1);
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

void Operator::setupServers()
{
	for (size_t i = 0; i < _servers.size(); i++)
	{
		_servers[i].continueSetup(_servers, i);
		_poFD.addFd(_servers[i].getSockFd());
	}
}

void	Operator::dataOnServer(int i)
{
	Client clie(i, _servers[i].sockAccept());
	_poFD.addFd(clie.getSock());
	_clients.push_back(clie);
}


void	Operator::dataOnClient(int i)
{
	int cIndex = lookClient(_poFD.getPfd()[i].fd);
	std::vector<unsigned char> request;
	request = _servers[_clients[cIndex].getIndex()].sockRecv(i, _poFD);
	if (request.size() <= 0)
	{
		std::vector<Client>::iterator it(_clients.begin());
		for (int m = 0; m < cIndex; m++)
			it++;
		_clients.erase(it);		
	} else {
		try	{
			RequestChecker(request, cIndex);
			}catch(const std::exception& e)	{
				Response tmpRSP;
				_clients[cIndex].setResp(tmpRSP.createErrorResponse(g_error, _servers[_clients[cIndex].getIndex()]));
				if (g_error != 200)
					g_error = 200;
				_clients[cIndex].clearRequest();
			}
			if (_clients[cIndex].getRFlag())
			{
				RequestParser RP(_clients[cIndex].tmpReq);
				int sIndex;
				sIndex = find_server(RP.getPort());
				Handler H(RP, _clients[cIndex]);
				H.start_handling(_servers[sIndex], _clients[cIndex]);
			}
			request.clear();
		}
}

void	Operator::dataToSend(int i)
{
	int cIndex;
	if ((cIndex = lookClient(_poFD.getPfd()[i].fd)) != -1)
	{
		if (_clients[cIndex].getResponseSize() > 0)
		{
			_servers[_clients[cIndex].getIndex()].sockSend(_poFD.getPfd()[i].fd, _clients[cIndex]);
			if (_clients[cIndex].getResponseSize() == 0)
			{
				if (_clients[cIndex].getStatusCode() == "413"){
					close(_poFD.getPfd()[i].fd);
					_poFD.deleteFd(i);
				}
				_clients[cIndex].resetClient();
			}							
		}
	}	
}

void Operator::start_process()
{
	g_error = 200;

	for (size_t i = 0; i < _servers.size(); ++i)
		_servers[i].bindPort();
	while(1)
	{
		poll(_poFD.getPfd().data(), _poFD.getFdCount(), 0); 
		if (!_poFD.getFdCount())
			setupServers();
		for (size_t i = 0; i < _poFD.getFdCount(); i++)
		{
			if (_poFD.getPfd()[i].revents & POLLIN)
			{
				int k;
				if ((k = fdServer(_poFD.getPfd()[i].fd)) != -1)
					dataOnServer(k);
				else
					dataOnClient(i);
			}
			if (i < _poFD.getFdCount())
				if (_poFD.getPfd()[i].revents & POLLOUT)
					dataToSend(i);
		}
	}
}
