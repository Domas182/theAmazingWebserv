#include "Server.hpp"

Server::Server()
{
	_index = "index.html";
	_limit_body = 10000000;
	sn = false;
	ip = false;
	p = false;
	r = false;
	i = false;
	am = false;
	lb = false;
	l = false;
}

Server::~Server()
{}

void Server::setLocation(std::vector<Location> location)
{
	_locations = location;
}

uint32_t check_uint32(std::string word)
{
	for (std::string::const_iterator it = word.begin(); it != word.end(); ++it)
	{
		if (!isdigit(*it))
			return (FAILURE);
	}
	uint32_t num = strtoul(word.c_str(), NULL, 10);
	if (num > 4294967295)
		return (FAILURE);
	return (SUCCESS);
}

void Server::setServerName(std::string server_name)
{
	server_name.erase(remove(server_name.begin(), server_name.end(), ';'));
	this->_server_name = server_name;
}

int Server::setIpAddress(std::string ip_address)
{
	int a = 999,b = 999,c = 999, d = 999;
	char ch;

	ip_address.erase(remove(ip_address.begin(), ip_address.end(), ';'));
	if (count(ip_address.begin(), ip_address.end(), '.') != 3)
		return (FAILURE);
	std::string tmp = ip_address;
	char *token = strtok(const_cast<char*>(tmp.c_str()), ".");
	while (token != NULL)
	{
		if (check_uint32(token))
			return (FAILURE);
		token = strtok(NULL, ".");
	}
	std::stringstream s(tmp);
	s >> a >> ch >> b >> ch >> c >> ch >> d;
	if (a > 255 || a < 0 || b > 255 || b < 0 || c > 255 || c < 0 || d > 255 || d < 0)
		return (FAILURE);
	this->_ip_address = ip_address;
	return (SUCCESS);
}

int Server::setPort(std::string port)
{
	uint32_t tmp;
	port.erase(remove(port.begin(), port.end(), ';'));
	std::stringstream s(port);
	s >> tmp;
	if (tmp > 65536 || tmp < 0 || check_uint32(port))
		return (FAILURE);
	this->_port = atol(port.c_str());
	return (SUCCESS);
}

int Server::setRoot(std::string root)
{
	root.erase(remove(root.begin(), root.end(), ';'));
	if (count(root.begin(), root.end(), '/') < 1)
		return (FAILURE);
	this->_root = root;
	return (SUCCESS);
}

int Server::setIndex(std::string index)
{
	index.erase(remove(index.begin(), index.end(), ';'));
	if (count(index.begin(), index.end(), '.') != 1)
		return (FAILURE);
	this->_index = index;
	return (SUCCESS);
}

int Server::setMethods(std::string methods)
{
	methods.erase(remove(methods.begin(), methods.end(), ';'));
	std::string tmp = methods;
	char *token = strtok(const_cast<char*>(tmp.c_str()), ",");
	while (token != NULL)
	{
		if ((strcmp(token, "GET") == 0) || (strcmp(token, "POST") == 0) || (strcmp(token, "DELETE") == 0))
		{
			_allowed_methods.push_back(token);
			token = strtok(NULL, ",");
		}
		else
			return (FAILURE);
	}
	return (SUCCESS);
}

int Server::setLimitBody(std::string limit)
{
	uint32_t tmp;
	limit.erase(remove(limit.begin(), limit.end(), ';'));
	std::stringstream s(limit);
	s >> tmp;
	if (tmp < 0 || check_uint32(limit))
		return (FAILURE);
	this->_limit_body = atol(limit.c_str());
	return (SUCCESS);
}

const std::string &Server::getServerName() const
{
	return (this->_server_name);
}

const std::string &Server::getIpAddress() const
{
	return (this->_ip_address);
}

uint32_t Server::getPort() const
{
	return (this->_port);
}

const std::string &Server::getRoot() const
{
	return (this->_root);
}

const std::string &Server::getIndex() const
{
	return (this->_index);
}

const std::vector<std::string> &Server::getMethods() const
{
	return (this->_allowed_methods);
}

uint32_t Server::getLimitBody() const
{
	return (this->_limit_body);
}

const std::vector<Location>	&Server::getLocation() const { return _locations; }

// // ************************************************************************** //
// //                               Location                                     //
// // ************************************************************************** //

void Location::setProxy(std::string proxy)
{
	proxy.erase(remove(proxy.begin(), proxy.end(), ';'));
	_proxy = proxy;
	_root = proxy;
}

int Location::setLocMethods(std::string methods)
{
	methods.erase(remove(methods.begin(), methods.end(), ';'));
	std::string tmp = methods;
	char *token = strtok(const_cast<char*>(tmp.c_str()), ",");
	while (token != NULL)
	{
		if ((strcmp(token, "GET") == 0) || (strcmp(token, "POST") == 0) || (strcmp(token, "DELETE") == 0))
		{
			_loc_methods.push_back(token);
			token = strtok(NULL, ",");
		}
		else
			return (FAILURE);
	}
	return (SUCCESS);
}

int Location::setIndex(std::string index)
{
	index.erase(remove(index.begin(), index.end(), ';'));
	if (count(index.begin(), index.end(), '.') != 1)
		return (FAILURE);
	this->_index = index;
	return (SUCCESS);
}

int Location::setRoot(std::string root)
{
	root.erase(remove(root.begin(), root.end(), ';'));
	if (count(root.begin(), root.end(), '/') < 1)
		return (FAILURE);
	this->_root = root;
	return (SUCCESS);
}

int Location::setDirectoryListing(std::string listing)
{
	listing.erase(remove(listing.begin(), listing.end(), ';'));
	if (listing == "on")
	{
		this->_dir_list = true;
		return (SUCCESS);
	}
	else if (listing == "off")
	{
		this->_dir_list = false;
		return (SUCCESS);
	}
	else
		return (FAILURE);
}

const std::string &Location::getProxy() const
{
	return (this->_proxy);
}

const std::vector<std::string> &Location::getLocMethods() const
{
	return (this->_loc_methods);
}

const std::string &Location::getIndex() const
{
	return (this->_index);
}

const std::string &Location::getRoot() const
{
	return (this->_root);
}

bool Location::getDirectoryListing() const
{
	return (this->_dir_list);
}