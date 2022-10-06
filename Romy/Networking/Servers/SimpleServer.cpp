#include "SimpleServer.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

SimpleServer::SimpleServer(int domain, int type, int protocol, int port, u_long interface, int blog )
{
		this->socket = new ListeningSocket(domain, type, protocol, port, interface, blog);
}


SimpleServer::~SimpleServer()
{
}


ListeningSocket * SimpleServer::getSocket(){
	return this->socket;
}
