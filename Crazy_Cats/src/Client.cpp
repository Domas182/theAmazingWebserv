#include "../incl/Client.hpp"

extern int	g_error;

bool crlfBool(std::vector<unsigned char>& data, size_t i)
{
	if (data[i] == '\r' && data[i + 1] == '\n')
		return true;
	else
		return false;	
}

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
	_headerTooBig = false;		
	_tmpCnt = 0;
	isEmpty = false;
	tmpLen = 0;
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

bool Client::getH2BFlag()
{
	return(_headerTooBig);
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

void Client::setH2BFlagT()
{
	_headerTooBig = true;
}

void Client::setH2BFlagF()
{
	_headerTooBig = false;
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

void Client::pushRequest(unsigned char c)
{
    _request.push_back(c);
}

void Client::pushBody(unsigned char c)
{
	_body.push_back(c);
}

void Client::printRequest()
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

void Client::printBody()
{
	for (size_t i = 0; i < tmpBody.size(); i++)
		std::cout << LB << tmpBody[i];
	std::cout << RESET << std::endl;
}

void	Client::clearRequest()
{
	_request.clear();
}

int Client::findBodyLength(std::vector<unsigned char>& request)
{
	RequestParser RP(request);
	std::unordered_map<std::string, std::string>::const_iterator got = RP.getRequestH().find("Content-Length");
	std::unordered_map<std::string, std::string>::const_iterator endit = RP.getRequestH().end();
	if (got != endit)
	{
		size_t contLen = atoi(got->second.c_str());
		this->tmpLen = contLen;
		return (contLen);
	}
	std::unordered_map<std::string, std::string>::const_iterator ITE = RP.getRequestH().find("Transfer-Encoding");
	if (ITE != endit)
		return (-1);
	return (0);
}

void Client::headerFlagSetter(int len)
{
	if (len > 0){
		setFlagT();
		setHBFlagT();
	}else if (len == -1 ){
		setFlagT();
		setCFlagT();
		setHBFlagT();
		chunkSizeSet = false;
	}else{
		setFlagT();
		setRFlagT();
	}
}

void Client::headerCountAndFlags(int len)
{
	setFlagT();
	len = findBodyLength(tmpReq);
	headerFlagSetter(len);
}



void Client::crlfPush()
{
	tmpReq.push_back('\r');
	tmpReq.push_back('\n');	
}

void Client::chunkedHandler(std::vector<unsigned char>& request, size_t& i, size_t bytes)
{

	while (i < bytes)
	{
		if (!chunkSizeSet)
		{
			while (i < bytes && !crlfBool(request, i))
				iHex.push_back(request[i++]);
			if (request[i] == '\r')
			{
				while (i < bytes && crlfBool(request, i))
					i += 2;
				chunkSize = std::stoi(iHex.c_str(), nullptr, 16);
				chunkSizeSet = true;
				iHex.clear();
			}
		}
		if (chunkSizeSet)
		{
			if (chunkSize == 0){
				setRFlagT();
			} else {
				while (tmpChunkedBody.size() < chunkSize && i < bytes)
					tmpChunkedBody.push_back(request[i++]);
				if (tmpChunkedBody.size() == chunkSize)
				{
					for (size_t x = 0; x < tmpChunkedBody.size(); x++)
						tmpBody.push_back(tmpChunkedBody[x]);
					chunkSizeSet = false;
					tmpChunkedBody.clear();
				}
				while (i < bytes && crlfBool(request, i))
					i += 2;
			}
		}
	}	
}


void	Client::resetClient()
{
	_requestFlag = false;
	_bodyFlag = false;
	_readyFlag = false;
	_hasBody = false;
	_isChunked = false;
	_headerTooBig = false;		
	_tmpCnt = 0;
	isEmpty = false;

	tmpReq.clear();
	tmpBody.clear();
	tmpExtract.clear();

	tmpLen = 0;
	g_error = 200;
}
