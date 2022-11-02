

#include "Handler.hpp"
#include <iostream>
#include <chrono>
#include <ctime>



void time_function()
{
	std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);
	std::cout << "finished computation at " << std::ctime(&end_time);
}

Handler::Handler(RequestParser RP, Client & client): _body(client.getBody()), _RP(RP)
{
	this->_method = _RP.getMethod();
	this->_URI = _RP.getURI();
	// this->_URI = "http://www.w3.org/favicon.ico/?password=password";
	this->_version = _RP.getVersion();
	this->_requestH = _RP.getRequestH();
	this->_path = "";
	this->_query = "";
	this->_port = "";
	this->_error_code = 200;
	client.printBody();
}
//TODO:can we put start_handling in the constructor??

Handler::~Handler()
{}

void	Handler::handle_get(Server & server, Client & client)
{

	if (server.set_Content(this->_path))
		throw std::invalid_argument("Error❗\nCould not open requested file");
		//TODO:404
	// this->_RSP.createResponse(200, server, this->_path, this->_version);
	client.setResp(this->_RSP.createResponse(200, server, this->_path, this->_version));
}


void Handler::write_file(std::vector<unsigned char> & input, std::string filename)
{
	std::fstream file;
	file.open(filename, std::ios_base::out);
	//should we protect that?
	for (size_t i = 0; i < input.size(); i++)
		file << input[i];
	file.close();
}

void Handler::pure_body(std::string & fileBody, Client& client)
{
	std::string rn = "\r\n\r\n";
	size_t pos = 0;
	if((pos = fileBody.find(rn)) != std::string::npos)
	{
		fileBody.erase(0, pos + rn.length());
		std::string test;
		rn = "\r\n";
		if ((pos = fileBody.find(rn)) != std::string::npos)
			test = fileBody.substr(0, pos);
		std::copy(test.begin(), test.end(), std::back_inserter(client.tmpExtract));
	}
}
void	Handler::get_file_info(std::string& fileBody)
{
	std::string rn = "\r\n";
	size_t pos = 0;
	size_t pos2 = 0;
	while ((pos = fileBody.find(rn)) != std::string::npos)
	{
		std::string test = fileBody.substr(0, pos);
		std::string delimeter = ":";
		if ((pos2 = test.find(delimeter) != test.size()))
		{
			std::cout << LB<< ":::::::::::::::::" RESET << std::endl;
			std::string key = test.substr(0, pos2);
			if (key == "Content-Disposition:")
			{

				this->_bodyHeader.insert(std::pair<std::string, std::vector<std::string> >(key, std::vector<std::string>()));
				//while loop der die ganzen atrtibute n die map parsed
				test.erase(0, pos2 + delimeter.length());
				delimeter = ";";
				std::string value = "";
				while((pos2 = test.find(delimeter)) != test.size())
				{
					std::string value = test.substr(0, pos2);
					this->_bodyHeader[key].push_back(value);
					test.erase(0, pos2 + delimeter.length());
					std::cout << ORANGE << "keyvalues:" << value << RESET << std::endl;
				}
				delimeter = "=";
				if ((pos2 = value.find(delimeter)) != value.size())
				{
					this->_filename = value.substr(pos2, value.size());
				}
			}
			else if (key == "Content-Type:")
			{
					std::string value = test.substr(0, test.size());
					this->_bodyHeader.insert(std::pair<std::string, std::vector<std::string> >(key, std::vector<std::string>()));
					this->_bodyHeader[key].push_back(value);
					std::cout << PINK << "keyvalues:" << value << RESET << std::endl;
			}
		}
	}
	// _bodyHeader["Content-Disposition:"]
	//wie aknn cih das letzet element vom map vector accessen?
}

void Handler::body_extractor(Client& client)
{
	// for(size_t x = 0; x < client.tmpBody.size(); x++)
	// {
	// 	std::cout << PINK << client.tmpBody[x] << RESET;
	// }
	std::cout << std::endl;
	std::string fileBody(client.tmpBody.begin(), client.tmpBody.end());
	get_file_info(fileBody);
	pure_body(fileBody, client);
	for (size_t x = 0; x < client.tmpExtract.size(); x++)
		std::cout << GREEN << client.tmpExtract[x] << RESET;

}
void	Handler::handle_post(Server & server, Client & client)
{

	if (client.getHBFlag())
	{
		body_extractor(client);
		write_file(client.tmpExtract, this->_filename);
	}
}

void	Handler::handle_methods(Server & server, Client & client)
{
	if (this->_method == "GET")
		handle_get(server, client);
	else if (this->_method == "POST")
		handle_post(server, client);
	// else if (this->_method == "DELETE")
	// 	handle_delete(server);
	

}
void	Handler::start_handling(Server & server, Client & client)
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
			// if (server.getPortStr() != this->_port)
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
	// if (server.getCgi() == "no")
		handle_methods(server, client);
	// else
	// 	Cgi CGI(server, client, _path, _query, _type, _RP);
}

void	Handler::change_path(Server & server)
{
	bool file_req = false;
	_path = server.getRoot() + _path;
	if (_URI.find(".") != STREND)
	{
		size_t path_start = _URI.find ('.');
		_type = _URI.substr(path_start + 1);
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
