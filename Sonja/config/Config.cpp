#include "Config.hpp"
#include "Server.hpp"
#include <vector>

Config::Config(std::string infile_name): _servers(), _loc(), _infile_name(infile_name.c_str()), _infile()
{
	_infile.open(_infile_name);
	if (!_infile.is_open())
		throw std::invalid_argument("Error❗\nCould not open config file");
}

Config::Config()
{}

Config::~Config(void)
{
	_infile.close();
	return;
}

void	clear_input(std::string &line)
{
	if (line.find_first_of("#") != STREND)
	{
		size_t i = line.find_first_of("#");
		if (i != STREND)
			line.erase(i);
	}
}

std::vector<std::string>	get_tokens(std::string line)
{
	char *token = strtok(const_cast<char*>(line.c_str()), " \t");
	std::vector<std::string> tokens;

	while (token != NULL)
	{
		tokens.push_back(std::string(token));
		token = strtok(NULL, " \t");
	}
	return (tokens);
}

void Config::parse_location(std::vector<std::string> tokens, Location &location)
{
	std::string line;

	if (tokens.size() != 2)
			throw std::invalid_argument("Error❗\nInvalid information in config file. No proxy defined at location block.");
	else
		location.setProxy(tokens.back());
	location.i = false;
	location.am = false;
	location.r = false;
	location.dl = false;
	while (getline(_infile, line))
	{
		clear_input(line);
		std::vector<std::string> tokens = get_tokens(line);
		if (!tokens.empty() && tokens.at(0) == "}")
			break;
		else if (!tokens.empty() && tokens.at(0) == "allowed_methods")
		{
			if (tokens.size() < 2 || tokens.size() > 4 || location.am == true)
				throw std::invalid_argument("Error❗\nInvalid information in config file for allowed methods in line:\n " + line);
			else
			{
				std::string methods;
				for (size_t i = 1; i < tokens.size(); i++)
					methods = methods + "," + tokens.at(i);
				if (location.setLocMethods(methods))
					throw std::invalid_argument("Error❗\nInvalid information in config file for allowed methods in line:\n " + line);
				location.am = true;
			}
		}
		else if (!tokens.empty() && tokens.at(0) == "index")
		{
			if (tokens.size() != 2 || location.setIndex(tokens.back()) || location.i == true)
				throw std::invalid_argument("Error❗\nInvalid information in config file for index in line:\n " + line);
			else
			{
				location.setIndex(tokens.back());
				location.i = true;
			}
		}
		else if (!tokens.empty() && tokens.at(0) == "root")
		{
			if (tokens.size() != 2 || location.setRoot(tokens.back()) || location.r == true)
				throw std::invalid_argument("Error❗\nInvalid information in config file for root in line:\n " + line);
			else
			{
				location.r = true;
				location.setRoot(tokens.back());
			}
		}
		else if (!tokens.empty() && tokens.at(0) == "directory_listing")
		{
			if (tokens.size() != 2 || location.setDirectoryListing(tokens.back()) || location.dl == true)
				throw std::invalid_argument("Error❗\nInvalid information in config file for directory listing in line:\n " + line);
			else
			{
				location.dl = true;
				location.setDirectoryListing(tokens.back());
			}
		}
	}
}

void Config::parse_server(Server &server)
{
	std::string line;

	if (!_loc.empty())
		_loc.clear();
	while (getline(_infile, line))
	{
		clear_input(line);
		std::vector<std::string> tokens = get_tokens(line);
		if (!tokens.empty() && tokens.at(0) == "}")
			break;
		else if (!tokens.empty() && tokens.at(0) == "server_name")
		{
				if (tokens.size() != 2 || server.sn == true)
					throw std::invalid_argument("Error❗\nInvalid information in config file for server name in line:\n " + line);
				else
				{
					server.setServerName(tokens.back());
					server.sn = true;
				}
		}
		else if (!tokens.empty() && tokens.at(0) == "ip-address")
		{
				if (tokens.size() != 2 || server.setIpAddress(tokens.back()) || server.ip == true)
					throw std::invalid_argument("Error❗\nInvalid information in config file for ip address in line:\n " + line);
				else
				{
					server.setIpAddress(tokens.back());
					server.ip = true;
				}
		}
		else if (!tokens.empty() && tokens.at(0) == "port")
		{
			if (tokens.size() != 2 || server.setPort(tokens.back()) || server.p == true)
				throw std::invalid_argument("Error❗\nInvalid information in config file for port in line:\n " + line);
			else
			{
				server.setPort(tokens.back());
				server.p = true;
			}
		}
		else if (!tokens.empty() && tokens.at(0) == "root")
		{
			if (tokens.size() != 2 || server.setRoot(tokens.back()) || server.r == true)
				throw std::invalid_argument("Error❗\nInvalid information in config file for root in line:\n " + line);
			else
			{
				server.r = true;
				server.setRoot(tokens.back());
			}
		}
		else if (!tokens.empty() && tokens.at(0) == "index")
		{
			if (tokens.size() != 2 || server.setIndex(tokens.back()) || server.i == true)
				throw std::invalid_argument("Error❗\nInvalid information in config file for index in line:\n " + line);
			else
			{
				server.setIndex(tokens.back());
				server.i = true;
			}
		}
		else if (!tokens.empty() && tokens.at(0) == "allowed_methods")
		{
			if (tokens.size() < 2 || tokens.size() > 4 || server.am == true)
				throw std::invalid_argument("Error❗\nInvalid information in config file for allowed methods in line:\n " + line);
			else
			{
				std::string methods;
				for (size_t i = 1; i < tokens.size(); i++)
					methods = methods + "," + tokens.at(i);
				if (server.setMethods(methods))
					throw std::invalid_argument("Error❗\nInvalid information in config file for allowed methods in line:\n " + line);
				server.am = true;
			}
		}
		else if (!tokens.empty() && tokens.at(0) == "limit_body")
		{
			if (tokens.size() != 2 || server.setLimitBody(tokens.back()) || server.lb == true)
				throw std::invalid_argument("Error❗\nInvalid information in config file for limit body in line:\n " + line);
			else
			{
				server.setLimitBody(tokens.back());
				server.am = true;
			}
		}
		else if (!tokens.empty() && tokens.at(0) == "location")
		{
			Location location;
			parse_location(tokens, location);
			_loc.push_back(location);
		}
	}
}

void Config::start_parsing()
{
	std::string line;

	while (getline(_infile, line))
	{
		clear_input(line);
		std::vector<std::string> tokens = get_tokens(line);
		if (tokens.size() > 1)
		{
			if (tokens.at(1) != "{")
				throw std::invalid_argument("Error❗\nInvalid information in config file for server in line:\n " + line);
		}
		if (!tokens.empty() && tokens.at(0) != "server")
			throw std::invalid_argument("Error❗\nConfig file does not start with server block");
		if (!tokens.empty() && tokens.at(0) == "server")
		{
			Server server;
			parse_server(server);
			server.setLocation(_loc);
			_servers.push_back(server);
		}
	}
}

const std::vector<Server>	&Config::getServer() const { return _servers; }

std::ostream	&operator<<(std::ostream &os, const Config &config) {

	int	j = 0;
	int	k = 0;
	std::set<std::string>::const_iterator it;
	for (std::vector<Server>::const_iterator it = config.getServer().begin(); it != config.getServer().end(); it++)
	{
		os << "\nSERVER: " << j << "\n";
		os << "Servers Name:	" << it->getServerName() << "\n";
		os << "IP Address:	" << it->getIpAddress() << "\n";
		os << "Port:		" << it->getPort() << "\n";
		os << "Root:		" << it->getRoot() << "\n";
		os << "Index:		" << it->getIndex() << "\n";
		os << "Methods:	";
		for (size_t i = 0; i < it->getMethods().size(); i++)
			os << it->getMethods().at(i) << " ";
		os << "\nLimit Body:	" << it->getLimitBody() << "\n";
		Server server = config.getServer().at(j);
		std::set<std::string>::const_iterator it2;
		for (std::vector<Location>::const_iterator it2 = server.getLocation().begin(); it2 != server.getLocation().end(); it2++)
		{
			os << "\n	LOCATION: " << k << " of SERVER: " << j << "\n";
			os << "	Proxy:		" << it2->getProxy() << "\n";
			os << "	Methods:	";
			for (size_t i = 0; i < it2->getLocMethods().size(); i++)
				os << it2->getLocMethods().at(i) << " ";
			os << " ";
			k++;
			os << "\n	Index:		" << it2->getIndex() << "\n";
			os << "	Root:		" << it2->getRoot() << "\n";
			os << "	directory listing:	" << it2->getDirectoryListing() << "\n";
		}
		j++;
	}
	return os;
}
