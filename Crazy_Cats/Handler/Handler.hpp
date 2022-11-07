
#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <string>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <vector>

#include "../src/Server.hpp"
#include "../Response/Response.hpp"
#include "../Request/RequestParser.hpp"
#include "../src/Client.hpp"
#include "../src/Cgi.hpp"



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
		std::vector<std::string>	_allowed_methods;
		std::string	_URI;
		std::string	_version;
		std::string	_path;
		std::string	_query;
		std::string	_port;
		std::string	_host;
		std::string	_type;
		std::string	_req_type;
		std::string _filename;
		std::unordered_map<std::string, std::string> _requestH;
		std::vector<unsigned char> & _body;
		std::map<std::string, std::vector<std::string> > _bodyHeader;
		Response  _RSP;
		RequestParser	_RP;
		bool		_file_req;

	public:
		Handler(RequestParser RP, Client & client);
		~Handler();


		void	check_methods();
		void		start_handling(Server & server, Client & client);
		void		change_path(Server & server);
		void		handle_get(Server & server, Client & client);
		void		handle_post(Server & server, Client & client);
		void 		body_extractor(Client& client);
		void		get_file_info(std::string& fileBody);
		void 		pure_body(std::string & fileBody, Client& client);
		void 		write_file(std::vector<unsigned char> & input, std::string filename);

		// void	handle_post(Server & server);
		// void	handle_delete(Server & server);
		void	handle_methods(Server & server, Client & client);
};

#endif /* HANDLER_HPP */