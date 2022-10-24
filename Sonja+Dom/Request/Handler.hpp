
#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <string>
#include <iostream>
#include <algorithm>
#include <unordered_map>

#include "RequestParser.hpp"
#include "../src/Server.hpp"

#define SUCCESS 0
#define FAILURE 1
#define STREND std::string::npos

// ************************************************************************** //
//                               Handler Class                                //
// ************************************************************************** //

class Handler
{
	private:
		std::string	_method;
		std::string	_URI;
		std::string	_version;
		std::string	_path;
		std::string	_query;
		std::string	_port;
		std::string	_host;
		int			_error_code; // 200 is default
		std::unordered_map<std::string, std::string> _requestH;

	public:
		Handler(RequestParser RP);
		~Handler();

		void	start_handling(Server server);
		void	change_path(Server server);
};

#endif /* HANDLER_HPP */