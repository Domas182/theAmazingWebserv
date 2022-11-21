#include "../incl/Operator.hpp"

Operator::Operator(std::string infile_name): _servers(), _loc(), _infile_name(infile_name.c_str()), _infile()
{}

Operator::Operator()
{}

Operator::~Operator(void)
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

void Operator::parse_location(std::vector<std::string> tokens, Location &location)
{
	std::string line;

	if (tokens.size() != 2)
			throw std::invalid_argument("Error❗\nInvalid information in Config file. No proxy defined at location block.");
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
				throw std::invalid_argument("Error❗\nInvalid information in Config file for allowed methods in line:\n " + line);
			else
			{
				std::string methods;
				for (size_t i = 1; i < tokens.size(); i++)
					methods = methods + "," + tokens.at(i);
				if (location.setLocMethods(methods))
					throw std::invalid_argument("Error❗\nInvalid information in Config file for allowed methods in line:\n " + line);
				location.am = true;
			}
		}
		else if (!tokens.empty() && tokens.at(0) == "index")
		{
			if (tokens.size() != 2 || location.setIndex(tokens.back()) || location.i == true)
				throw std::invalid_argument("Error❗\nInvalid information in Config file for index in line:\n " + line);
			else
			{
				location.setIndex(tokens.back());
				location.i = true;
			}
		}
		else if (!tokens.empty() && tokens.at(0) == "root")
		{
			if (tokens.size() != 2 || location.setRoot(tokens.back()) || location.r == true)
				throw std::invalid_argument("Error❗\nInvalid information in Config file for root in line:\n " + line);
			else
			{
				location.r = true;
				location.setRoot(tokens.back());
			}
		}
		else if (!tokens.empty() && tokens.at(0) == "directory_listing")
		{
			if (tokens.size() != 2 || location.setDirectoryListing(tokens.back()) || location.dl == true)
				throw std::invalid_argument("Error❗\nInvalid information in Config file for directory listing in line:\n " + line);
			else
			{
				location.dl = true;
				location.setDirectoryListing(tokens.back());
			}
		}
	}
}

void Operator::parse_server(Server &server)
{
	std::string line;
	std::string methods;
	std::string cgi;

	if (!_loc.empty())
		_loc.clear();
	while (getline(_infile, line))
	{
		clear_input(line);
		std::vector<std::string> tokens = get_tokens(line);
		if (!tokens.empty() && tokens.at(0) == "}")
		{
			server.open_bracket = false;
			break;
		}
		else if (!tokens.empty() && tokens.at(0) == "server_name")
		{
				if (tokens.size() != 2 || server.sn == true)
					throw std::invalid_argument("Error❗\nInvalid information in Config file for server name in line:\n " + line);
				else
				{
					server.setServerName(tokens.back());
					server.sn = true;
				}
				if(!_servers.empty())
				{
					for (std::vector<Server>::const_iterator it = _servers.begin(); it != _servers.end(); it++)
					{
						if (it->getServerName() == server.getServerName())
							throw std::invalid_argument("Error❗\nConfig file contains the same server name multiple times.\n" + line);
					}
				}
		}
		else if (!tokens.empty() && tokens.at(0) == "port")
		{
			if (tokens.size() != 2 || server.setPort(tokens.back()) || server.p == true)
				throw std::invalid_argument("Error❗\nInvalid information in Config file for port in line:\n " + line);
			server.p = true;
			if(!_servers.empty())
			{
				for (std::vector<Server>::const_iterator it = _servers.begin(); it != _servers.end(); it++)
				{
					if (it->getPort() == server.getPort())
						throw std::invalid_argument("Error❗\nConfig file contains the same port number multiple times.\n" + line);
				}
			}
		}
		else if (!tokens.empty() && tokens.at(0) == "root")
		{
			if (tokens.size() != 2 || server.setRoot(tokens.back()) || server.r == true)
				throw std::invalid_argument("Error❗\nInvalid information in Config file for root in line:\n " + line);
			else
			{
				server.r = true;
				server.setRoot(tokens.back());
			}
		}
		else if (!tokens.empty() && tokens.at(0) == "index")
		{
			if (tokens.size() != 2 || server.setIndex(tokens.back()) || server.i == true)
				throw std::invalid_argument("Error❗\nInvalid information in Config file for index in line:\n " + line);
			else
			{
				server.setIndex(tokens.back());
				server.i = true;
			}
		}
		else if (!tokens.empty() && tokens.at(0) == "allowed_methods")
		{
			if (tokens.size() < 2 || tokens.size() > 4 || server.am == true)
				throw std::invalid_argument("Error❗\nInvalid information in Config file for allowed methods in line:\n " + line);
			else
			{
				for (size_t i = 1; i < tokens.size(); i++)
					methods = methods + "," + tokens.at(i);
				if (server.setMethods(methods))
					throw std::invalid_argument("Error❗\nInvalid information in Config file for allowed methods in line:\n " + line);
				server.am = true;
			}
		}
		else if (!tokens.empty() && tokens.at(0) == "limit_body")
		{
			if (tokens.size() != 2 || server.setLimitBody(tokens.back()) || server.lb == true)
				throw std::invalid_argument("Error❗\nInvalid information in Config file for limit body in line:\n " + line);
			else
			{
				server.setLimitBody(tokens.back());
				server.lb = true;
			}
		}
		else if (!tokens.empty() && tokens.at(0) == "error_pages")
		{
			if (tokens.size() != 2 || server.setErrorPages(tokens.back()) || server.ep == true)
				throw std::invalid_argument("Error❗\nInvalid information in Config file for cgi in line:\n " + line);
			else
			{
				server.ep = true;
				server.setErrorPages(tokens.back());
			}
		}
		else if (!tokens.empty() && tokens.at(0) == "cgi")
		{
			if (tokens.size() < 2 || tokens.size() > 3 || server.c == true)
				throw std::invalid_argument("Error❗\nInvalid information in Config file for cgi in line:\n " + line);
			else
			{
				for (size_t i = 1; i < tokens.size(); i++)
					cgi = cgi + "," + tokens.at(i);
				if (server.setCgi(cgi))
					throw std::invalid_argument("Error❗\nInvalid information in Config file for cgi in line:\n " + line);
				server.c = true;
			}
		}
		else if (!tokens.empty() && tokens.at(0) == "location")
		{
			Location location;
			parse_location(tokens, location);
			if (location.getLocMethods().empty())
			{
				if (methods.empty())
					location.setLocMethods(",GET,POST,DELETE;");
				else
					location.setLocMethods(methods);
			}
			_loc.push_back(location);
		}
	}
}

void Operator::check_data()
{
	int	j = 0;
	if (_servers.size() > 10)
		throw std::invalid_argument("Error❗\nToo many servers! Maximum 10 servers.");
	if (_servers.size() < 1)
		throw std::invalid_argument("Error❗\nNo server defined in Config file. Minimum 1 server.");
	for (std::vector<Server>::const_iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		if (it->getServerName().empty())
			throw std::invalid_argument("Error❗\nServer name is missing in Config file.");
		if (!it->getPort())
			throw std::invalid_argument("Error❗\nPort is missing in Config file.");
		Server server = getServer().at(j);
		std::set<std::string>::const_iterator it2;
		for (std::vector<Location>::const_iterator it2 = server.getLocation().begin(); it2 != server.getLocation().end(); it2++)
		{
			if (it2->getIndex().empty())
				throw std::invalid_argument("Error❗\nIndex is missing in location block.");
		}
		j++;
	}
}

void Operator::start_parsing()
{
	std::string line;

	_infile.open(_infile_name);
	if (!_infile.is_open())
		throw std::invalid_argument("Error❗\nCould not open Config file");

	while (getline(_infile, line))
	{
		clear_input(line);
		std::vector<std::string> tokens = get_tokens(line);
		if (tokens.size() > 1)
		{
			if (tokens.at(1) != "{")
				throw std::invalid_argument("Error❗\nInvalid information in Config file for server in line:\n " + line);
		}
		if (!tokens.empty() && tokens.at(0) != "server")
		{
			if (_servers.empty())
				throw std::invalid_argument("Error❗\nConfig file does not start with server block.");
			else
				throw std::invalid_argument("Error❗\nInvalid content in Config file outside of server block.");

		}
		if (!tokens.empty() && tokens.at(0) == "server")
		{
			Server server;
			parse_server(server);
			if (server.open_bracket == true)
				throw std::invalid_argument("Error❗\nServer block in Config file is not closed.");
			server.setLocation(_loc);
			_servers.push_back(server);
		}
	}
	check_data();
}

const std::vector<Server>	&Operator::getServer() const { return _servers; }

std::ostream	&operator<<(std::ostream &os, const Operator &Operator) {

	int	j = 0;
	std::set<std::string>::const_iterator it;
	for (std::vector<Server>::const_iterator it = Operator.getServer().begin(); it != Operator.getServer().end(); it++)
	{
		int	k = 0;
		os << "\nSERVER: " << j << "\n";
		os << "Servers Name:	" << it->getServerName() << "\n";
		os << "Port:		" << it->getPort() << "\n";
		os << "Root:		" << it->getRoot() << "\n";
		os << "Index:		" << it->getIndex() << "\n";
		os << "Methods:	";
		for (size_t i = 0; i < it->getMethods().size(); i++)
			os << it->getMethods().at(i) << " ";
		os << "\nLimit Body:	" << it->getLimitBody() << "\n";
		os << "\nCgi:	";
		for (size_t i = 0; i < it->getCgi().size(); i++)
			os << it->getCgi().at(i) << " ";
		os << "\nError Pages:	" << it->getErrorPages() << "\n";
		Server server = Operator.getServer().at(j);
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
