//an object of a socket

#ifndef SIMPLE_SOCKET
#define SIMPLE_SOCKET

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

class CoonectingSocket;

class SimpleSocket{
	//bind the socket to the network - give it a name
	//listening is handled by the server
	private:

		struct sockaddr_in _address;
		int _sock;
		//user shouldnt be able to manipulate the connection
		//we just want to be able to see what it is and reference it
		int	_connection;
	public:

	//define/ create the socket
		SimpleSocket(int domain, int type, int protocol, int port, u_long interface);
		//1st param: AF_INET = domain of the socket: the type of IP address
		//2nd param: SOCK_STREAM = type of socket (SOCK_STREAM for webservers)
		//3rd param: different versions of protocols, in our case 0

	//virtual function to choose between binding and conencting
	virtual int establish_connection(int sock, struct sockaddr_in address ) = 0;
	//not there yet *
	void		test_connection(int sock_to_test);
	struct sockaddr_in getAddress()const;
	int				getSock()const;
	void			setConnection(int conn);
};
#endif

//* now we need a derived class