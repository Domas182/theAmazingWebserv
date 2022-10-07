#include "Server.hpp"

Server::Server()
{}

Server::~Server()
{}

void Server::setLocation(Location &location)
{
	_locations.push_back(location);
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
	_server_name = server_name;
}

int Server::setIpAddress(std::string ip_address)
{
	int a = 999,b = 999,c = 999, d = 999;
	char ch;

	ip_address.erase(remove(ip_address.begin(), ip_address.end(), ';'));
	if (count(ip_address.begin(), ip_address.end(), '.') != 3)
		return (FAILURE);
	char *token = strtok(const_cast<char*>(ip_address.c_str()), ".");
	while (token != NULL)
	{
		if (check_uint32(token))
			return (FAILURE);
		token = strtok(NULL, ".");
	}
	std::stringstream s(ip_address);
	s >> a >> ch >> b >> ch >> c >> ch >> d;
	if (a > 255 || a < 0 || b > 255 || b < 0 || c > 255 || c < 0 || d > 255 || d < 0)
		return (FAILURE);
	_ip_address = ip_address;
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
	_port = atol(port.c_str());
	return (SUCCESS);
}

const std::string Server::getServerName() const
{
	return (_server_name);
}

const std::string Server::getIpAddress() const
{
	return (_ip_address);
}
