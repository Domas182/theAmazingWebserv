#include "Client.hpp"

Client::Client() {}

Client::Client(int index, int socket) : _socket(socket), _sIndex(index)
{
	fcntl(_socket, F_SETFL, O_NONBLOCK);
	_statCode = '0';
	_requestFlag = false;
	_bodyFlag = false;
	_readyFlag = false;
	_hasBody = false;
	_isChunked = false;
	_tmpCnt = 0;
}

Client::~Client(){}

int &Client::getSock()
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

bool Client::getBFlag()
{
	return(_bodyFlag);
}

bool Client::getRFlag()
{
	return(_readyFlag);
}

bool Client::getHBFlag()
{
	return(_hasBody);
}

bool Client::getCFlag()
{
	return(_isChunked);
}

void Client::setFlagT()
{
	_requestFlag = true;
}

void Client::setFlagF()
{
	_requestFlag = false;
}

void Client::setBFlagF()
{
	_bodyFlag = false;
}

void Client::setBFlagT()
{
	_bodyFlag = true;
}

void Client::setRFlagF()
{
	_readyFlag = false;
}

void Client::setRFlagT()
{
	_readyFlag = true;
}

void Client::setHBFlagF()
{
	_hasBody = false;
}

void Client::setHBFlagT()
{
	_hasBody = true;
}

void Client::setCFlagT()
{
	_isChunked = true;
}

void Client::setCFlagF()
{
	_isChunked = false;
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
	// std::cout << resp << std::endl;
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
std::vector<unsigned char> & Client::getBody()
{
	return (_body);
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

void Client::eraseSentBit()
{
	_response.erase(_response.begin(), _response.begin() + _sentBytes);
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

void    Client::pushBody(unsigned char c)
{
    _body.push_back(c);
}

void    Client::printRequest()
{
    for (size_t i = 0; i < tmpReq.size(); i++)
        std::cout << tmpReq[i];
    std::cout << std::endl;
}

size_t Client::getBodySize()
{
	return(_body.size());
}

RequestParser & Client::getRP()
{
	return (_RP);
}

void    Client::printBody()
{
    for (size_t i = 0; i < tmpBody.size(); i++)
        std::cout << LB << tmpBody[i];
    std::cout << RESET << std::endl;
}

void	Client::clearRequest()
{
	_request.clear();
}