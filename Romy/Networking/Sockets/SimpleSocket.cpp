#include "SimpleSocket.hpp"
//TODO:delete this!
const int trueFlag = 1;
//Default Constructor
SimpleSocket::SimpleSocket(int domain, int type, int protocol, int port, u_long interface)
{//define our address structure
	_address.sin_family = domain;
		//1st param: AF_INET = domain of the socket: the type of IP address
	_address.sin_port = htons(port);
		//we need to convert the  bytes of the port 
	_address.sin_addr.s_addr = htonl(interface);
//set the interface on which this socket is running
//again convert the bytes of the value we get
//that's what hton is for

//establish socket
	_sock = socket(domain, type, protocol);
	setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, &trueFlag, sizeof(int));
	test_connection(_sock);
//establish network connection
	// _connection = CoonectingSocket::establish_connection(_sock, _address);
	// test_connection(_connection);
//u have to test them one at a time

}
/* define address_struct
, give domain port interface, establish the socket, connection will be made (bind/connect)
*/

//test connection virtual function
//confirms that either socket or connection has been properly established
void	SimpleSocket::test_connection(int sock_to_test){

	if (sock_to_test < 0)
	{
		perror("failed to connect socket");
		//error msg probably change in the future
		exit(EXIT_FAILURE);
		//maybe make a virtual exit function later
	}
}
	struct sockaddr_in SimpleSocket::getAddress()const{

		return _address;
	}
	int				SimpleSocket::getSock()const{
		return _sock;
	}

	void				SimpleSocket::setConnection(int conn){

		_connection = conn;
	}
