#include "BindingSocket.hpp"

BindingSocket::BindingSocket(int domain, int type, int protocol, int port, u_long interface): 
	SimpleSocket(domain, type, protocol, port, interface){
		setConnection(establish_connection(getSock(), getAddress()));
}
BindingSocket::~BindingSocket(){};

int BindingSocket::establish_connection(int sock, struct sockaddr_in address){
	// sock = getSock();
	// address = getAddress();
	return bind(sock, (struct sockaddr *)&address, sizeof(address));

}
