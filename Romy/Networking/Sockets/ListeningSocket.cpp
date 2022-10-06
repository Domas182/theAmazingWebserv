#include "ListeningSocket.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ListeningSocket::ListeningSocket(int domain, int type, int protocol, int port, u_long interface, int blog): 
BindingSocket(domain, type, protocol, port, interface)
{
	backlog = blog;
	start_listening();
	test_connection(listening);
}


ListeningSocket::~ListeningSocket()
{
}

void ListeningSocket::start_listening()
{
	listening = listen(getSock(), backlog);
}
