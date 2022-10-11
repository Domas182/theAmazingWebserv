
#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <map>
#include <vector>
#include <set>
#include <fstream>
#include <sstream>

#include "Server.hpp"
#include "Location.hpp"
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
	std::vector<Server>	_servers;
	std::string			_infile_name;
	std::ifstream		_infile;
	// nmbr of servers

	void	parse_server(Server server);
	void	parse_location(Location location, std::vector<std::string> tokens);

	public:
	Config();
	Config(std::string infile);
	~Config(void);
	
	const std::vector<Server>	&getServer() const;

	// Config & operator=(Config const & rhs); //copy assignment operator overload

	void	start_parsing();
};

std::ostream	&operator<<(std::ostream &os, const Config &config);

#endif /* CONFIG_HPP */
