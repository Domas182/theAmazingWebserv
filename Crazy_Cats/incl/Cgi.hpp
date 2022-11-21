
#ifndef CGI_HPP
#define CGI_HPP

#include "CrazyCats.hpp"
#include "Server.hpp"
#include "Response.hpp"
#include "Client.hpp"
#include "RequestParser.hpp"

// ************************************************************************** //
//                               Cgi Class                                 //
// ************************************************************************** //

class Cgi
{
	private:
		std::unordered_map<std::string, std::string>	_env;
		std::string										_exec_str;
		std::string										_path;
		std::string										_query;
		std::string										_method;
		std::string										_type;
		std::string										_content_len;
		std::string										_path_info;
		std::string										_content_type;
		RequestParser									_RP;
		std::string										_body;
		bool											_error;
		char *											body;
		FILE *											in; 
		FILE *											tmp;
		std::string										_response;

	public:
		Cgi(Server & server, std::string path, std::string query,
			std::string type, RequestParser & RP, std::string _body_send);
		~Cgi();
		void		set_Env(Server & server);
		void		set_exec_str();
		void		process(char ** env_str);
		void		CgiResponse(Server & server);
		void		create_Response(Server & server, std::string read);
		std::string	const & getResponse() const;
		bool		const & getError() const;
		std::string set_time();
};

#endif /* CGI_HPP */