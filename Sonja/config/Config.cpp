#include "Config.hpp"

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
	//delete comment in line
	if (line.find_first_of("#"))
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
		// std::cout << "TOKENXXXXX   " << tokens.at(0) << std::endl;
		// if (line.find("}") == 0)
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
		// std::cout << "BEFORE   " << line << std::endl;
		clear_input(line);
		// std::cout << "AFTER   " << line << std::endl;
		std::vector<std::string> tokens = get_tokens(line);
		// std::cout << "TOKEN   " << tokens.at(1) << std::endl;
		// if (line.find("}") == 0)
		if (!tokens.empty() && tokens.at(0) == "}")
			break;
		// else if (line.find("location") == 0)
		else if (!tokens.empty() && tokens.at(0) == "location")
		{
			Location* location = new Location();
			server.locations.push_back(*location);
			parse_location(*location, tokens);
		}
		// else
		// {
		// }
	}
}

void Config::start_parsing()
{
	std::string line;

	while (getline(_infile, line))
	{
		// std::cout << "BEFORE   " << line << std::endl;
		clear_input(line);
		std::vector<std::string> tokens = get_tokens(line);
		// std::cout << "AFTER   " << line << std::endl;
		// std::cout << "TOKEN   " << tokens.at(0) << std::endl;
		// std::cout << "TOKEN   " << tokens.at(1) << std::endl;
		if (tokens.size() != 1)
		{
			if (tokens.at(1) != "{")
				throw std::invalid_argument("Error❗\nInvalid information in config file at server line");
		}
		if (!tokens.empty() && tokens.at(0) != "server")
			throw std::invalid_argument("Error❗\nConfig file does not start with server block");
		if (!tokens.empty() && tokens.at(0) == "server")
		{
			Server* server = new Server();
			_servers.push_back(*server);
			parse_server(*server);
		}
		// if (line.find("server") == 0)
		// {
		// 	Server* server = new Server();
		// 	_servers.push_back(*server);
		// 	parse_server(*server);
		// }
	}
}
