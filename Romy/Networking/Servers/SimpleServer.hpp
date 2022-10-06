 #ifndef SIMPLESERVER_HPP
# define SIMPLESERVER_HPP

# include <iostream>
# include <string>
# include "../lib_networking.hpp"


class SimpleServer
{

	public:

		SimpleServer(int domain, int type, int protocol, int port, u_long interface, int blog);
		~SimpleServer();
		virtual void launch() = 0;
		//will call he other 3
		ListeningSocket * getSocket();
	protected:

		ListeningSocket *socket;
		virtual void request_listener() = 0;
		virtual void handler() = 0;
		virtual void responder() = 0;

};


#endif /* **************************************************** SIMPLESERVER_H */