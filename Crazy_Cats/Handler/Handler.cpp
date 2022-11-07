

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

extern int	g_error;

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
	client.printBody();
}

Handler::~Handler()
{}


void Handler::write_file(std::vector<unsigned char> & input, std::string filename)
{
	// for (size_t i = 0; i < input.size(); i++)
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
		fileBody.erase(0, pos + rn.length());
		std::string delimeter = ":";
		if ((pos2 = test.find(delimeter)) != std::string::npos)
		{
			std::string key = test.substr(0, pos2);
			if (key == "Content-Disposition")
			{
				std::cout << LB<< key<<  RESET << std::endl;

				this->_bodyHeader.insert(std::pair<std::string, std::vector<std::string> >(key, std::vector<std::string>()));
				test.erase(0, pos2 + delimeter.length());
				delimeter = ";";
				std::string value = "";
				while((pos2 = test.find(delimeter)) != std::string::npos)
				{
					value = test.substr(0, pos2);
					this->_bodyHeader[key].push_back(value);
					test.erase(0, pos2 + delimeter.length());
				}
				delimeter = "=";
				if ((pos2 = test.find(delimeter)) != std::string::npos)
				{
					std::string testfile = test.substr((pos2 + 2), (test.size() - pos2 -3));
					this->_filename = testfile;
					std::cout << ORANGE << "keyvalues:" << this->_filename << RESET << std::endl;
				}
			}
			else if (key == "Content-Type")
			{
					std::string value = test.substr(pos2 + 1, test.size());
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
	std::string fileBody(client.tmpBody.begin(), client.tmpBody.end());
	get_file_info(fileBody);
	std::string fileBody2(client.tmpBody.begin(), client.tmpBody.end());

	pure_body(fileBody2, client);

}

void	Handler::handle_post(Server & server, Client & client)
{	
	std::cout << GREEN << client.getHBFlag() << "\t" << client.getCFlag() << RESET;
	server.set_Content(this->_path, 1);
		// throw std::invalid_argument("Error❗\nCould not open requested file");
		// //TODO:404
	if (client.getHBFlag() && !client.getCFlag())
	{
		body_extractor(client);
		client.setResp(this->_RSP.createResponse(201, server, this->_path, this->_version));
		write_file(client.tmpExtract, this->_filename);
	} else if (client.getHBFlag() && client.getCFlag()){
		client.setResp(this->_RSP.createResponse(201, server, this->_path, this->_version));
		write_file(client.tmpBody, "chunked.txt");		
	}
}

void	Handler::handle_get(Server & server, Client & client)
{
	if (g_error == 200)
		server.set_Content(this->_path, 1);
	if (g_error != 200)
		server.set_Content(this->_path, g_error);
	client.setResp(this->_RSP.createResponse(g_error, server, this->_path, this->_version));
	if (g_error != 200)
		g_error = 200;
}

void	Handler::handle_methods(Server & server, Client & client)
{
	std::cout << GREEN << this->_method << RESET;
	if (this->_method == "GET")
		handle_get(server, client);
	else if (this->_method == "POST")
	{
		handle_post(server, client);
	}
	// else if (this->_method == "DELETE")
	// 	handle_delete(server);
	

}

void	Handler::check_methods()
{
	bool check = false;
	if (_file_req == false)
	{
		if (_method == "HEAD" || _method == "PUT" || _method == "CONNECT" || _method == "OPTIONS" || _method == "TRACE")
		{
			g_error = 501;
			return ;
		}
		for (std::vector<std::string>::const_iterator it = _allowed_methods.begin(); it != _allowed_methods.end(); it++)
		{
			if (it->data() == _method)
				check = true;
		}
		if (check == false)
			g_error = 405;
	}
}

void	Handler::start_handling(Server & server, Client & client)
{
	std::unordered_map<std::string, std::string>::const_iterator got = _requestH.find("Host");
	if (got == _requestH.end())
		g_error = 400;
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
	check_methods();
	if (server.getCgi() == "no")
		handle_methods(server, client);
	else
		Cgi CGI(server, client, _path, _query, _type, _RP);
}

void	Handler::change_path(Server & server)
{
	this->_allowed_methods = server.getMethods();
	_file_req = false;
	_path = server.getRoot() + _path;
	if (_URI.find(".") != STREND)
	{
		size_t path_start = _URI.find ('.');
		_type = _URI.substr(path_start + 1);
		_file_req = true;
	}
	if (server.getLocation().empty() && _file_req == false)
	{
		_path = _path + server.getIndex();
		this->_allowed_methods = server.getMethods();
	}
	if (!server.getLocation().empty())
	{
		std::string tmp;
		for (std::vector<Location>::const_iterator it = server.getLocation().begin(); it != server.getLocation().end(); it++)
		{
			if (_file_req == false)
			{
				if (this->_URI == it->getProxy())
				{
					_path = server.getRoot() + it->getRoot() + it->getIndex();
					this->_allowed_methods = it->getLocMethods();
					return;
				}
				if (std::count(_URI.begin(), _URI.end(), '/') > 1)
				{
					tmp = _URI.substr(1);
					size_t end = tmp.find('/');
					tmp = _URI.substr(0, end + 1);
					std::string tmp2 = _URI.substr(end + 2);
					if (tmp == it->getProxy())
					{
						_path = server.getRoot() + it->getRoot() + tmp2 + it->getIndex();
						this->_allowed_methods = it->getLocMethods();
					}
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
					this->_allowed_methods = it->getLocMethods();
				}
			}
		}
	}
	std::ifstream input_file;
	input_file.open(_path);
	if (!input_file.is_open())
		g_error = 404;
}
