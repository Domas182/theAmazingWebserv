#include "Cgi.hpp"

Cgi::Cgi(Server server, Client & client, std::string path, std::string query, std::string type, RequestParser RP) : 
	_path(path), _query(query), _type(type), _RP(RP)
{
	std::cout << "HERE" << std::endl;
	_method = _RP.getMethod();
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
	_env["SERVER_PORT"] = server.getPortStr();
	_env["SERVER_PROTOCOL"] = "HTTP/1.1";

	if (_method == "POST")
	{
		_env["CONTENT_LENGTH"] = _content_len;
		_env["CONTENT_TYPE"] = _type;
	}

	std::string tmp;
	for(std::unordered_map<std::string, std::string>::const_iterator it = _RP.getRequestH().begin();
	it != _RP.getRequestH().end(); ++it)
	{
		tmp = it->first;
		for (size_t i = 0; i < tmp.length(); i++)
			tmp[i] = std::tolower(tmp[i]);
		_env["HTTP_" + tmp] = it->second;
	}
	// PRINT ENV
	// for(std::unordered_map<std::string, std::string>::const_iterator it = _env.begin();
	// it != _env.end(); ++it)
	// {
	// 	std::cout << it->first << "		" << it->second << std::endl;
	// }

}