#include "Cgi.hpp"

Cgi::Cgi(Server server, Client & client, std::string path, std::string query, std::string method, std::string type) : 
	_path(path), _query(query), _method(method), _type(type)
{
	set_Env(server);
}

Cgi::~Cgi()
{}

void Cgi::set_Env(Server server)
{
	uint32_t len = server.getContent().length();
	std::stringstream ss;
	ss << len;
	_content_len = ss.str();
	_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	_env["PATH_INFO"] = _path;
	_env["PATH_TRANSLATED"] = _path;
	_env["QUERY_STRING"] = _query;
	_env["REDIRECT_STATUS"] = "200";
	_env["REQUEST_METHOD"] = _method;
	_env["SERVER_NAME"] = server.getServerName();
	_env["SERVER_PORT"] = server.getPort();
	_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	_env["CONTENT_LENGTH"] = _content_len;
	_env["CONTENT_TYPE"] = _type;

	// std::vector<std::string, std::string>::const_iterator it = _RP.begin();
	// while (it != _RP.end()) {
	// 	_env["HTTP_" + it->first] = it->second;
	// 	it++;
	// }
}