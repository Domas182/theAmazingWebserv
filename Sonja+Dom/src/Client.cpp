#include "Client.hpp"

Client::Client() {}

Client::Client(int index, int socket) : _socket(socket), _sIndex(index)
{
	fcntl(_socket, F_SETFL, O_NONBLOCK);
	_statCode = '0';
	_requestFlag = false;
	_tmpCnt = 0;
}

Client::~Client(){}

int Client::getSock()
{
	return(_socket);
}

int Client::getIndex()
{
	return(_sIndex);
}

bool Client::getFlag()
{
	return(_requestFlag);
}

void Client::setFlagT()
{
	_requestFlag = true;
}

void Client::setFlagF()
{
	_requestFlag = false;
}

int Client::getCnt()
{
	return(_tmpCnt);
}

void Client::setCnt(int i)
{
	_tmpCnt = i;
}

void Client::setResp(std::string resp)
{
	_response = resp;
	_bytesToSend = _response.size();
	_totalSentBytes = 0;
}

size_t Client::getResponseSize()
{
	return(_response.size());
}

std::vector<unsigned char> & Client::getRequest()
{
	return (_request);
}


void Client::setSentBytes(int bytes)
{
	_sentBytes = bytes;
}

std::string Client::getResponse()
{
	return(_response);
}

void Client::setTotalSentBytes()
{
	_totalSentBytes += _sentBytes;
}

void Client::clearResponse()
{
	_response.clear();
}

void Client::setStatusCode(std::string string)
{
	_statCode = string;
}

std::string Client::getStatusCode()
{
	return (_statCode);
}

void    Client::pushRequest(unsigned char c)
{
    _request.push_back(c);
}

void    Client::printRequest()
{
    for (size_t i = 0; i < _request.size(); i++)
        std::cout << _request[i];
	//for break condition
	if (_request[5] == 'f')
	{
		std::cout << std::endl;
	}
    std::cout << std::endl;
}