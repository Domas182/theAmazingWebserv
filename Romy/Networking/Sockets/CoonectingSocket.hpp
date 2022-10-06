#ifndef COONECTINGSOCKET_HPP
# define COONECTINGSOCKET_HPP

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include "SimpleSocket.hpp"

class CoonectingSocket: public SimpleSocket
{

	public:

		CoonectingSocket(int domain, int type, int protocol, int port, u_long interface);
		~CoonectingSocket();
		int establish_connection(int sock, struct sockaddr_in address);


	private:

};

#endif /* ************************************************ COONECTINGSOCKET_H */