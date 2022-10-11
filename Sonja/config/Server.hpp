
#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <iostream>
#include <algorithm>
#include <sstream>

#include <vector>
#include <set>
#include <map>

#include "Location.hpp"
#define SUCCESS 0
#define FAILURE 1
#define STREND std::string::npos

// ************************************************************************** //
//                               Server Class                                //
// ************************************************************************** //

class Location;

class Server
{
	private:
		std::string					_server_name;
		std::string					_ip_address;
		uint32_t					_port;
		std::string					_root;
		std::string					_index;
		std::set<std::string>		_allowed_methods;
		std::string					_errorpages;
		unsigned long				_limit_body;
		std::vector<Location>		_locations;

		void		default_init();

	public:
		Server();
		~Server();
		
		// const std::vector<Location>	getLocation() const;
		const std::string			getServerName() const;
		const std::string			getIpAddress() const;
		uint32_t					getPort() const;
		const std::string			getRoot() const;
		const std::string			getIndex() const;

		void						setLocation(Location location);
		void						setServerName(std::string server_name);
		int							setIpAddress(std::string ip_address);
		int							setPort(std::string port);
		int							setRoot(std::string root);
		int							setIndex(std::string idnex);
		int							setMethods(std::set<std::string> methods);
};

#endif /* SERVER_HPP */