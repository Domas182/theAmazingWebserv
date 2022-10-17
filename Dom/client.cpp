#include "client.hpp"

Client::Client(){}

Client::Client(int index, int socket) : _sIndex(index), _socket(socket)
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

void Client::setSentBytes(int bytes)
{
    _sentBytes = bytes;
}

std::string Client::getResponse()
{
    return(_response);
}

void Client::setTotalSentBytes(int bytes)
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