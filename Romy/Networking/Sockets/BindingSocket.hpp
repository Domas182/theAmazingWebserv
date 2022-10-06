#ifndef BINDINGSOCKET_HPP
#define BINDINGSOCKET_HPP

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include "SimpleSocket.hpp"

class BindingSocket: public SimpleSocket
{

	public:

		BindingSocket(int domain, int type, int protocol, int port, u_long interface);
		~BindingSocket();
		int establish_connection(int sock, struct sockaddr_in address);
};
#endif 