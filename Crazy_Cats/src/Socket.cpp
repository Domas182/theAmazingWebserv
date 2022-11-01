
#include "Socket.hpp"
#include "../Request/RequestParser.hpp"

ft_tsocket::ft_tsocket(){}

ft_tsocket::ft_tsocket(uint16_t port) : _port(port), nbytes(0)
{
	_socketFd = socket(AF_INET, SOCK_STREAM, 0);
	_address.sin_family = AF_INET;
	_address.sin_port = htons(_port);
	_address.sin_addr.s_addr = INADDR_ANY;
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
	listen(_socketFd, 10);
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
	sBytes = send(fd, &client.getResponse()[0], client.getResponseSize(), 0);
	//sBytes = send(fd, &client.getResponse(), client.getResponseSize(), MSG_NOSIGNAL);
	client.setSentBytes(sBytes);
	client.setTotalSentBytes();
	client.clearResponse();
}

std::vector<unsigned char> ft_tsocket::socketRecv(int i, PollFd &tPoll)
{
	std::vector<unsigned char> buf(65536);
	//TODO:buf(x) == our maxread size is not the same as maxbody size == still missing!
	nbytes = recv(tPoll.getPfd()[i].fd, &buf[0], buf.size(), MSG_DONTWAIT);
	if (nbytes <= 0)
	{
		close(tPoll.getPfd()[i].fd);
		tPoll.deleteFd(i);
	}
	return (buf);
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

