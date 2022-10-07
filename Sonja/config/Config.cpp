#include "Config.hpp"
#include "Server.hpp"

Config::Config(std::string infile_name): _servers(), _infile_name(infile_name.c_str()), _infile()
{
	_infile.open(_infile_name);
	if (!_infile.is_open())
		throw std::invalid_argument("Error❗\nCould not open config file");
}

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

void Config::parse_location(Location &location, std::vector<std::string> tokens)
{
	std::string line;

	if (tokens.size() != 2)
			throw std::invalid_argument("Error❗\nInvalid information in config file. No proxy defined at location block.");
	while (getline(_infile, line))
	{
		clear_input(line);
		std::vector<std::string> tokens = get_tokens(line);

		if (!tokens.empty() && tokens.at(0) == "}")
			break;
		location.root = "4565";
	}
}

void Config::parse_server(Server &server)
{
	std::string line;
	while (getline(_infile, line))
	{
		clear_input(line);
		std::vector<std::string> tokens = get_tokens(line);
		if (!tokens.empty() && tokens.at(0) == "}")
			break;
		else if (!tokens.empty() && tokens.at(0) == "server_name")
		{
				if (tokens.size() != 2)
					throw std::invalid_argument("Error❗\nInvalid information in config file for server name in line:\n " + line);
				else
					server.setServerName(tokens.back());
		}
		else if (!tokens.empty() && tokens.at(0) == "ip-address")
		{
				if (tokens.size() != 2 || server.setIpAddress(tokens.back()))
					throw std::invalid_argument("Error❗\nInvalid information in config file for ip address in line:\n " + line);
				else
					server.setIpAddress(tokens.back());
		}
		else if (!tokens.empty() && tokens.at(0) == "port")
		{
			if (tokens.size() != 2 || server.setPort(tokens.back()))
				throw std::invalid_argument("Error❗\nInvalid information in config file for port in line:\n " + line);
			else
				server.setPort(tokens.back());
		}
		else if (!tokens.empty() && tokens.at(0) == "location")
		{
			Location location = Location();
			server.setLocation(location);
			parse_location(location, tokens);
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
			std::cout << "TOKEN   " << tokens.size() << "  " << tokens.at(0) << std::endl;
		if (tokens.size() > 1)
		{
			if (tokens.at(1) != "{")
				throw std::invalid_argument("Error❗\nInvalid information in config file for server in line:\n " + line);
		}
		if (!tokens.empty() && tokens.at(0) != "server")
			throw std::invalid_argument("Error❗\nConfig file does not start with server block");
		if (!tokens.empty() && tokens.at(0) == "server")
		{
			Server server = Server();
			_servers.push_back(server);
			parse_server(server);
		}
	}
}
