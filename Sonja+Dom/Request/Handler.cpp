#include "Handler.hpp"
#include "RequestParser.hpp"
#include "../src/Client.hpp"


Handler::Handler(RequestParser RP, Client & client): _body(client.getBody())
{
	this->_method = RP.getMethod();
	this->_URI = RP.getURI();
	// this->_URI = "http://www.w3.org/favicon.ico/?password=password";
	this->_version = RP.getVersion();
	this->_requestH = RP.getRequestH();
	this->_path = "";
	this->_query = "";
	this->_port = "";
	this->_error_code = 200;
	client.printBody();

}
//TODO:can we put start_handling in the constructor??

Handler::~Handler()
{}


void	Handler::handle_get(Server & server)
{
	if (server.set_Content(this->_path))
		throw std::invalid_argument("Error❗\nCould not open requested file");
	server.set_Response(this->_path);
}

void	Handler::start_handling(Server & server)
{
	std::unordered_map<std::string, std::string>::const_iterator got = _requestH.find("Host");
	if (got == _requestH.end())
		throw std::runtime_error("No Host in Request"); //this is only temporary -> set error page instead
		// this->_error_code = 400;
	else
	{
		std::string port;
		size_t port_start = got->second.find(":");
		this->_host = got->second.substr(1, (port_start - 1));
		// if (server.getServerName() != this->_host)
		// 	throw std::runtime_error("Expected host does not match requested host"); //this is only temporary -> set error page instead
		if (got->second.find(":") != STREND)
			this->_port = got->second.substr(port_start + 1);
		if (this->_port == "")
			this->_port = 80;
		else
		{
			std::stringstream ss;
			ss << server.getPort();
			std::string port_str = ss.str();
			// if (port_str != this->_port)
			// 	throw std::runtime_error("Expected port does not match requested port"); //this is only temporary -> set error page instead
		}
		if (!this->_URI.find("http://"))
		{
			size_t path_start = _URI.find('/', 7);
			if (_URI.find("?") != STREND)
			{
				size_t query_start = _URI.find('?', 7);
				this->_path = _URI.substr(7, std::min(path_start, query_start) - 7);
				this->_query = _URI.substr(query_start);
			}
			else
				this->_path = _URI.substr(7);
		}
		else
		{
			size_t path_start = _URI.find('/');
			size_t query_start = 0;
			if (_URI.find('?') != STREND)
			{
				size_t query_start = _URI.find('?');
				this->_query = _URI.substr(query_start);
			}
			this->_path = _URI.substr(std::min(path_start, query_start));
		}
		if (this->_path[0] == '/')
			this->_path = this->_path.substr(1);
		// std::cout << "HOST " << this->_host << std::endl;
		// std::cout << "QURY " << _query << std::endl;
		// std::cout << "PATH " << _path << std::endl;
		// std::cout << "PORT " << this->_port << std::endl;
		// std::cout << "URI " << this->_URI << std::endl;
	}
	change_path(server);
	if (this->_method == "GET")
		handle_get(server);
}

void	Handler::change_path(Server & server)
{
	bool file_req = false;
	_path = server.getRoot() + _path;
	if (_URI.find(".") != STREND)
	{
		size_t path_start = _URI.find ('.');
		std::string type = _URI.substr(path_start + 1);
		file_req = true;
	}
	if (server.getLocation().empty() && file_req == false)
		_path = _path + server.getIndex();
	if (!server.getLocation().empty())
	{
		std::string tmp;
		for (std::vector<Location>::const_iterator it = server.getLocation().begin(); it != server.getLocation().end(); it++)
		{
			if (file_req == false)
			{
				if (this->_URI == it->getProxy())
				{
					_path = server.getRoot() + it->getRoot() + it->getIndex();
					return;
				}
				if (std::count(_URI.begin(), _URI.end(), '/') > 1)
				{
					tmp = _URI.substr(1);
					size_t end = tmp.find('/');
					tmp = _URI.substr(0, end + 1);
					std::string tmp2 = _URI.substr(end + 2);
					if (tmp == it->getProxy())
						_path = server.getRoot() + it->getRoot() + tmp2 + it->getIndex();
				}
				_path = _path + server.getIndex();
			}
			else
			{
				if (std::count(_URI.begin(), _URI.end(), '/') > 1)
				{
					size_t start = _URI.rfind('/');
					tmp = _URI.substr(start + 1);
					_path = server.getRoot() + tmp;
				}
			}
		}
	}
	std::cout << "{{{{{{{{{ " << _path << std::endl;
}
