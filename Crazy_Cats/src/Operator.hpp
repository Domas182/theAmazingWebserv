
#ifndef OPERATOR_HPP
#define OPERATOR_HPP

#include <string>
#include <sys/socket.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <cstdlib>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <poll.h>
#include <fstream>
#include <sstream>
#include <sys/wait.h>
#include <vector>

#include "Server.hpp"
#include <iostream>
#include <stdexcept>
#include <iterator>

#define SUCCESS 0
#define FAILURE 1
#define STREND std::string::npos


// ************************************************************************** //
//                               Operator Class                               //
// ************************************************************************** //

class Server;

class Operator
{
	private:
	std::vector<Server>		_servers;
	std::vector<Client>		_clients;
	std::vector<Location>	_loc;
	std::string				_infile_name;
	std::ifstream			_infile;
	PollFd					_poFD;

	void	parse_server(Server &server);
	void	parse_location(std::vector<std::string> tokens, Location &location);
	void	check_data();

	public:
	Operator();
	Operator(std::string infile);
	~Operator(void);

	const std::vector<Server>	&getServer() const;

	void	start_parsing();
	void	start_process();
	int		fdServer(int fd);
	int 	lookClient(int fd);
	int		find_server(uint32_t port);
	void	setupServers();
	void	RequestChecker(std::vector<unsigned char>& request, int c);
	void 	dataOnServer(int i);
	void	dataOnClient(int i);
	void	dataToSend(int i);


};

std::ostream	&operator<<(std::ostream &os, const Operator &Operator);

#endif /* OPERATOR_HPP */
