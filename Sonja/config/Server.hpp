
#ifndef SERVER_HPP
#define SERVER_HPP

#include <functional>
#include <ostream>
#include <memory>
#include <stdint.h>
#include <string>

#include <vector>
#include <set>
#include <map>

#include "Location.hpp"

// ************************************************************************** //
//                               Server Class                                //
// ************************************************************************** //

class Location;

class Server
{
	private:
		std::string						_server_name;
		uint32_t						_ip_address;
		std::vector<int>				_ports;
		std::string						_root;
		std::string						_index;
		std::set<std::string>			_allowed_methods;
		std::string						_errorpages;
		long							_limit_body;

		void		default_init();

	public:
		Server();
		~Server();
		std::vector<Location>				locations;
		
		const std::string			&getServerName() const;

		void						setServerName(std::string &server_name);

	//operation overload schreiben zum printen der Daten
};

#endif /* SERVER_HPP */