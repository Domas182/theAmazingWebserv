#ifndef TESTSERVER_HPP
# define TESTSERVER_HPP

# include <iostream>
# include <string>
# include <sstream>
# include "SimpleServer.hpp"
# include "../Request/RequestParser.hpp"

#define PINK "\033[1;35m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define LB		"\033[1;34m"
#define RED		"\033[1;31m"
#define RESET  "\033[0m"

#define BLACK "\033[0;30m"
#define CYAN "\033[0;36m"
#define WHITE "\033[0;37m" 

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