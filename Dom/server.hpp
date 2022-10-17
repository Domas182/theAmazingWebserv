#ifndef SERVER_HPP
#define SERVER_HPP
#include <sys/socket.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <cstdlib>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <poll.h>
#include <fstream>
#include <sstream>
#include <sys/wait.h>
#include <vector>
#include "socket.hpp"
#include "poller.hpp"
#include "client.hpp"

class ft_tsocket;

class   Server
{
    private:
        std::string _name;
        ft_tsocket  _sock;
        int         _port;
        size_t      _max_size;
    public:
        Server();
        Server(int port);
//        Server(std::vector<Server> servs, int port, int index);
        ~Server();

        void    continueSetup(std::vector<Server> servs, int index);
        bool portAvailabe(std::vector<Server> servs, int port, int i);
        int  getPort();
        int const&  getSockFd() const;
        int sockAccept();
        std::vector<unsigned char> sockRecv(int i, PollFd &oPoll);
        size_t getNBytes();
//        int    sockSend(int fd, std::string a);
        void sockSend(int fd, Client& client);


};

#endif