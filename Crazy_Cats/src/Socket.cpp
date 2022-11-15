
#include "Socket.hpp"
#include "../Request/RequestParser.hpp"

ft_tsocket::ft_tsocket(){}

ft_tsocket::ft_tsocket(uint16_t port) : _port(port), nbytes(0)
{
	_socketFd = socket(AF_INET, SOCK_STREAM, 0);
	_address.sin_family = AF_INET;
	_address.sin_port = htons(_port);
	_address.sin_addr.s_addr = INADDR_ANY;
	nbytes = 0;
	memset(_address.sin_zero, '\0', sizeof _address.sin_zero);
}

ft_tsocket::~ft_tsocket(){}

ft_tsocket &ft_tsocket::operator=(ft_tsocket const& data)
{
	_socketFd = data._socketFd;
	_port = data._port;
	_address = data._address;
	_addrlen = data._addrlen;
	return (*this);
}

void ft_tsocket::setToNonBlocking()
{
	fcntl(_socketFd, F_SETFL, O_NONBLOCK);
	const int trueFlag = 1;
	if (setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &trueFlag, sizeof(int)) < 0)
		perror("Failure");
}

void ft_tsocket::socketBind()
{
	if (bind(_socketFd, (struct sockaddr *)&_address, sizeof(_address)) < 0)
	{
		std::cout << "Failed to bind to port. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
}

void ft_tsocket::socketListen()
{
	listen(_socketFd, 100);
	std::cout << "Port: " << _port << " is listening." << std::endl;
}

int ft_tsocket::socketAccept()
{
	int addrlen = sizeof(_address);
	int nSock = accept(_socketFd, (struct sockaddr *)&_address, (socklen_t *)&addrlen);
	return (nSock);
}

//int ft_tsocket::socketSend(int fd, std::string answer)
//{
//    int sBytes = 0;
//    sBytes = send(fd, &answer[0], answer.size(), MSG_NOSIGNAL);
//    return (sBytes);
//}

void ft_tsocket::socketSend(int fd, Client& client)
{
	int sBytes = 0;
	std::string temp = client.getResponse();
	//std::cout << "get? size: " << client.getResponseSize() << std::endl;
	sBytes = send(fd, &client.getResponse()[0], client.getResponseSize(), 0);
	//sBytes = send(fd, &client.getResponse(), client.getResponseSize(), MSG_NOSIGNAL);
	client.setSentBytes(sBytes);
	client.setTotalSentBytes();
	client.eraseSentBit();
}

std::vector<unsigned char> ft_tsocket::socketRecv(int i, PollFd &tPoll)
{
	//std::vector<unsigned char> buf(150728640);
	std::vector<unsigned char> buf(65536);
	// std::vector<unsigned char> buf(8192);
	//std::vector<unsigned char> buf(8192);
	//TODO:buf(x) == our maxread size is not the same as maxbody size == still missing!
	nbytes = recv(tPoll.getPfd()[i].fd, &buf[0], buf.size(), MSG_DONTWAIT);
	// int total = 0;
	// while ((nbytes = recv(tPoll.getPfd()[i].fd, &buf[total], sizeof(buf) - total, MSG_DONTWAIT)) > 0)
	// 	total += nbytes;
	if (nbytes <= 0)
	{
		close(tPoll.getPfd()[i].fd);
		tPoll.deleteFd(i);
	}
	return (buf);
}


//testing new approach
std::vector<unsigned char> ft_tsocket::testRecv(int i, PollFd &tPoll)
{
	std::vector<unsigned char> buf(4);
	std::vector<unsigned char> temp;
	int j = 4;
	nbytes = recv(tPoll.getPfd()[i].fd, &buf[0], buf.size(), MSG_DONTWAIT);
	while (temp[j-3] != '\r' && temp[j-2] != '\n' && temp[j-1] != '\r' && temp[j] != '\n')
	{
		nbytes = recv(tPoll.getPfd()[i].fd, &buf[0], buf.size(), MSG_DONTWAIT);
		std::cout << buf;
		temp.push_back(buf[0]);
		j++;
	}
	if (nbytes <= 0)
	{
		close(tPoll.getPfd()[i].fd);
		tPoll.deleteFd(i);
	}
	return (temp);
}


struct sockaddr_in const& ft_tsocket::getAddress() const
{
	return (_address);
}

int const& ft_tsocket::getSocketFd() const
{
	return (_socketFd);
}

int ft_tsocket::getAddrlen()
{
	return (_addrlen);
}

