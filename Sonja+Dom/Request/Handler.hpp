
#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <string>
#include <iostream>
#include <algorithm>
#include <unordered_map>

#include "RequestParser.hpp"

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
		std::unordered_map<std::string, std::string> _requestH;

	public:
		Handler(RequestParser RP);
		~Handler();

		void	start_handling();
};

#endif /* HANDLER_HPP */