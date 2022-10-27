#include "Handler.hpp"
#include "RequestParser.hpp"

Handler::Handler(RequestParser RP)
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
}

Handler::~Handler()
{}


void	Handler::handle_get(Server & server)
{
	if (server.set_Content(this->_path))
		throw std::invalid_argument("Error❗\nCould not open requested file");
	if (server.setF_Content())
		throw std::invalid_argument("Error❗\nCould not open index file");
	if (server.setImg_Content())
		throw std::invalid_argument("Error❗\nCould not open image file");
	if (server.setFavi_Content())
		throw std::invalid_argument("Error❗\nCould not open image file");
	server.set_Response(this->_path);
	server.setResponse();
	server.setImg_Response();
	server.setFavi_Response();
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
		std::cout << "HOST " << this->_host << std::endl;
		std::cout << "QURY " << _query << std::endl;
		std::cout << "PATH " << _path << std::endl;
		std::cout << "PORT " << this->_port << std::endl;
		std::cout << "URI " << this->_URI << std::endl;
	}
	change_path(server);
	handle_get(server);
}

void	Handler::change_path(Server & server)
{
	_path = server.getRoot() + _path;
	if (server.getLocation().empty())
	{
		if (this->_URI == "/")
			_path = _path + server.getIndex();
	}
	else
	{
		std::string tmp;
		tmp = server.getRoot() + _path;
		for (std::vector<Location>::const_iterator it = server.getLocation().begin(); it != server.getLocation().end(); it++)
		{
			if (this->_URI == it->getProxy())
			{
				if (this->_URI == "/")
				{
					_path = _path + it->getIndex();
					break;
				}
				_path = server.getRoot() + it->getRoot() + it->getIndex();
				break;
			}
		}
	}
	// std::cout << "{{{{{{{{{ " << _path << std::endl;
}

