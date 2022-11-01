
#ifndef CGI_HPP
#define CGI_HPP

#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <sys/wait.h>

#include <vector>
#include <map>

// #include "../Request/RequestParser.hpp"
#include "../src/Server.hpp"
#include "../Response/Response.hpp"
#include "../src/Client.hpp"

// ************************************************************************** //
//                               Cgi Class                                 //
// ************************************************************************** //

class Cgi
{
	private:
		std::unordered_map<std::string, std::string>	_env;
		std::string							_path_translated;
		std::string							_path;
		std::string							_query;
		std::string							_method;
		std::string							_type;
		std::string							_content_len;
		std::string							_path_info;
		std::string							_content_type;
		RequestParser						_RP;
		// FILE*								_infile; 

	public:
		Cgi(Server server, Client & client, std::string path, std::string query, std::string type, RequestParser RP);
		~Cgi();

		void	set_Env(Server server);
};

#endif /* CGI_HPP */