#include "TestServer.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

TestServer::TestServer(): SimpleServer(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10)
{
	launch();
}


TestServer::~TestServer()
{
}

void TestServer::launch(){

	while(1)
	{
		std::cout << "<<<WAITING>>>" << "\n\n" << std::endl;
		request_listener();
		handler();
		responder();
		std::cout << "\n\n" << "<<<DONE>>>" << std::endl;

	}
}

void TestServer::request_listener(){
	struct sockaddr_in address = socket->getAddress();
	int addrlen = sizeof(address);
	new_socket = accept(socket->getSock(), (struct sockaddr *)&address, (socklen_t *)&addrlen);
	bzero(buffer, 30000);
	read(new_socket, buffer, 30000);
	//gibt es eine ander Methode als alles in einen Buffer zu schreiben??
	//TODO:schick den buffer in den request_parser
}
//accepts and reads the address

void TestServer::handler(){

	std::cout << buffer << std::endl;
	RequestParser Rp;
	Rp.split_CRLF(buffer);
	std::cout << Rp << std::endl;
}

void TestServer::responder(){

	std::stringstream ss;
	ss << "HELLO FROM ROMY";
	std::string hello = ss.str();
	write(new_socket, hello.c_str(), hello.size());
	close(new_socket);

}