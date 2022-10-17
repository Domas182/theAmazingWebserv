#include "server.hpp"

Server::Server(){}

Server::Server(int port) : _port(port) 
{
    _sock = ft_tsocket(_port);
    _sock.setToNonBlocking();
}

void Server::continueSetup(std::vector<Server> servs, int index)
{
    if (portAvailabe(servs, _port, index))
    {
        _sock.socketBind();
        _sock.socketListen();
    }
}

Server::~Server(){}

bool Server::portAvailabe(std::vector<Server> servs, int port, int i)
{
    for (int j = 0; j < i; j++)
        if(servs[j].getPort() == port)
            return (false);
    return (true);
}

int Server::getPort()
{
    return (_port);
}

int const& Server::getSockFd() const
{
    return (_sock.getSocketFd());
}

int Server::sockAccept()
{
    return (_sock.socketAccept());
}

std::vector<unsigned char> Server::sockRecv(int i, PollFd &oPoll)
{
    return(_sock.socketRecv(i, oPoll));
}

size_t Server::getNBytes()
{
    return(_sock.nbytes);
}

//int Server::sockSend(int fd, std::string answer)
//{
//    return(_sock.socketSend(fd, answer));
//}

void Server::sockSend(int fd, Client& client)
{
    _sock.socketSend(fd, client);
}


