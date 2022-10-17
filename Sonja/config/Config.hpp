
#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include "Server.hpp"
#include <iostream>
#include <stdexcept>
#include <iterator>

#define SUCCESS 0
#define FAILURE 1
#define STREND std::string::npos

// ************************************************************************** //
//                               Config Class                                //
// ************************************************************************** //

class Server;

class Config
{
	private:
	std::vector<Server>		_servers;
	std::vector<Location>	_loc;
	std::string				_infile_name;
	std::ifstream			_infile;

	void	parse_server(Server &server);
	void	parse_location(std::vector<std::string> tokens, Location &location);
	void	check_data();

	public:
	Config();
	Config(std::string infile);
	~Config(void);

	const std::vector<Server>	&getServer() const;

	void	start_parsing();
};

std::ostream	&operator<<(std::ostream &os, const Config &config);

#endif /* CONFIG_HPP */
