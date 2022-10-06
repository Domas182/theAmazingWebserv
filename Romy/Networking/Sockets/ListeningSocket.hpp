#ifndef LISTENINGSOCKET_HPP
# define LISTENINGSOCKET_HPP

# include <iostream>
# include <string>
# include "BindingSocket.hpp"

class ListeningSocket: public BindingSocket

{

	public:

		ListeningSocket(int domain, int type, int protocol, int port, u_long interface, int blog);
		~ListeningSocket();
		void start_listening();

	private:

		int backlog;
		int listening;

};

//backlog is the length of the waiting qeue so to say
#endif /* ************************************************* LISTENINGSOCKET_H */