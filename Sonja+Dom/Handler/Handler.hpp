
#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <string>
#include <iostream>
#include <algorithm>
#include <unordered_map>

#include "../src/Server.hpp"
#include "../Response/Response.hpp"
#include "../Request/RequestParser.hpp"
#include "../src/Client.hpp"



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
		std::vector<unsigned char> & _body;
		Response  _RSP;

	public:
		Handler(RequestParser RP, Client & client);
		~Handler();

		void	start_handling(Server & server, Client & client);
		void	change_path(Server & server);
		void	handle_get(Server & server, Client & client);
		// void	handle_post(Server & server);
		// void	handle_delete(Server & server);
		void	handle_methods(Server & server, Client & client);
};

#endif /* HANDLER_HPP */