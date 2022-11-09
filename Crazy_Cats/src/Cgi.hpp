
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
		std::string							_exec_str;
		std::string							_path;
		std::string							_query;
		std::string							_method;
		std::string							_type;
		std::string							_content_len;
		std::string							_path_info;
		std::string							_content_type;
		RequestParser						_RP;


		FILE *								in; 
		FILE *								tmp;
		std::string							_response;

	public:
		Cgi(Server & server, Client & client, std::string path, std::string query,
			std::string type, RequestParser & RP);
		~Cgi();

		void		set_Env(Server & server);
		void		process(char ** env_str);
		void		CgiResponse(Server & server, Client & client);
		void		create_Response(Server & server, std::string read);
		std::string	const & getResponse() const;
		std::string set_time();
};

#endif /* CGI_HPP */