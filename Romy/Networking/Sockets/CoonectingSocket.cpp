#include "CoonectingSocket.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CoonectingSocket::CoonectingSocket(int domain, int type, int protocol, int port, u_long interface): 
	SimpleSocket(domain, type, protocol, port, interface)
{
	setConnection(establish_connection(getSock(), getAddress()));
}

CoonectingSocket::~CoonectingSocket()
{
}
int CoonectingSocket::establish_connection(int sock, struct sockaddr_in address){
	// sock = getSock();
	// address = getAddress();
	return connect(sock, (struct sockaddr *)&address, sizeof(address));

}