#include "../incl/Operator.hpp"
#include "../incl/Server.hpp"
#include "../incl/Poller.hpp"
#include "../incl/Client.hpp"
#include "../incl/RequestParser.hpp"
#include "../incl/Handler.hpp"
#include "../incl/Response.hpp"

int	g_error;

void 	Operator::RequestChecker(std::vector<unsigned char>& request, int c)
{
	size_t i = 0;
	int contLen = 0;
	if (!_clients[c].getFlag())
	{
		while(i < _servers[_clients[c].getIndex()].getNBytes() && !_clients[c].getFlag() && request[i] != '\0')
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
	if (static_cast<uint32_t>(_clients[c].tmpLen) > _servers[_clients[c].getIndex()].getLimitBody() && !_clients[c].getCFlag())
		_clients[c].setRFlagT();
	if (_clients[c].getHBFlag() && _clients[c].getFlag() && !_clients[c].getRFlag())
	{
		if (!_clients[c].getRFlag() && !_clients[c].getCFlag())
		{
			while (i < _servers[_clients[c].getIndex()].getNBytes())
				_clients[c].tmpBody.push_back(request[i++]);
			if (_clients[c].tmpBody.size() == static_cast<uint32_t>(_clients[c].tmpLen))
				_clients[c].setRFlagT();
		}
		if (!_clients[c].getRFlag() && _clients[c].getCFlag())
			_clients[c].chunkedHandler(request, i, _servers[_clients[c].getIndex()].getNBytes());
	}
}
void	Operator::RequestSizeCheck(int c, int i)
{
	if(_clients[c].tmpReq.size() > 32768 && !_clients[c].getCFlag())
	{
		_clients[c].resetClient();
		_clients[c].setH2BFlagT();
		closeAndDelete(i);
		g_error = 413;
		throw std::runtime_error("Requestheader too big");
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

void	Operator::closeAndDelete(int i)
{
	close(_poFD.getPfd()[i].fd);
	_poFD.deleteFd(i);
}

void 	Operator::cleanUp()
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].isEmpty)
		{
			std::vector<Client>::iterator it(_clients.begin());
			for (size_t m = 0; m < i; m++)
				it++;
			_clients.erase(it);
		}
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

int	Operator::dataOnClient(int i)
{
	int cIndex = lookClient(_poFD.getPfd()[i].fd);
	if (cIndex == -1)
		return -1;
	std::vector<unsigned char> request;
	
	request = _servers[_clients[cIndex].getIndex()].sockRecv(i, _poFD);
	try	{
		RequestChecker(request, cIndex);
		RequestSizeCheck(cIndex, i);
		if (_clients[cIndex].tmpReq.size() == 0)
		{	
			_clients[cIndex].isEmpty = true;
			return (-1);
		}
		}catch(std::exception& e)
		{
				std::cerr << e.what() << '\n';
				Response tmpRSP;
				_clients[cIndex].setResp(tmpRSP.createErrorResponse(g_error, _servers[_clients[cIndex].getIndex()]));
				if (g_error != 200)
					g_error = 200;
				_clients[cIndex].clearRequest();
				_clients[cIndex].setRFlagF();
		}
		if (_clients[cIndex].getRFlag())
		{
			RequestParser RP(_clients[cIndex].tmpReq);
			int sIndex;
			sIndex = find_server(RP.getPort());
			Handler H(RP, _clients[cIndex]);
			H.start_handling(_servers[sIndex], _clients[cIndex]);
		}
		if (g_error != 200)
			g_error = 200;
		request.clear();
	return (0);
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
				if (_clients[cIndex].getStatusCode() == "413")
					closeAndDelete(i);
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
		try{
		poll(_poFD.getPfd().data(), _poFD.getFdCount(), 1000); 
		if (!_poFD.getFdCount())
			setupServers();
		for (size_t i = 0; i < _poFD.getFdCount(); i++)
		{
			if (_poFD.getPfd()[i].revents & POLLRDNORM)
			{
				int k;
				if ((k = fdServer(_poFD.getPfd()[i].fd)) != -1)
					dataOnServer(k);
				else
					dataOnClient(i);
			}
			if (i < _poFD.getFdCount())
				if (_poFD.getPfd()[i].revents & POLLWRNORM)
					dataToSend(i);
		}
		cleanUp();
		} catch (std::exception &e){
			g_error = 200;
			std::cerr << e.what() << std::endl;
		}
	}
}
