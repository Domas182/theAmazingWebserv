#ifndef TESTSERVER_HPP
# define TESTSERVER_HPP

# include <iostream>
# include <string>
# include <sstream>
# include "SimpleServer.hpp"
# include "../Request/RequestParser.hpp"

class TestServer: public SimpleServer
{

	public:

		TestServer();
		~TestServer();
		void launch();

	private:

		char buffer[30000];
		int new_socket;
		void request_listener();
		void handler();
		void responder();

};


#endif /* ****************************************************** TESTSERVER_H */