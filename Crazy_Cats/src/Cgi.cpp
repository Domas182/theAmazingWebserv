#include "Cgi.hpp"

Cgi::Cgi(Server server, Client & client, std::string path, std::string query, std::string type, RequestParser RP) : 
	_path(path), _query(query), _type(type), _RP(RP)
{
	_method = _RP.getMethod();
	set_Env(server);
}

Cgi::~Cgi()
{}

void free_array(char **input)
{
	int	i = 0;
	if (input == NULL)
		return ;
	while (input[i] != NULL)
	{
		free(input[i]);
		input[i] = NULL;
		i++;
	}
	free(input);
	input = NULL;
}

void Cgi::set_Env(Server server)
{
	_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	_env["PATH_INFO"] =_path;
	_env["PATH_TRANSLATED"] = _path;
	_env["QUERY_STRING"] = _query;
	_env["REDIRECT_STATUS"] = "200";
	_env["REQUEST_METHOD"] = _method;
	_env["SERVER_NAME"] = server.getServerName();
	_env["SERVER_PORT"] = server.getPortStr();
	_env["SERVER_PROTOCOL"] = "HTTP/1.1";

	if (_method == "POST")
	{
		uint32_t len = server.getContent().length();
		std::stringstream ss;
		ss << len;
		_content_len = ss.str();
		_env["CONTENT_LENGTH"] = _content_len;
		_env["CONTENT_TYPE"] = _type;
	}

	std::string tmp;
	for(std::unordered_map<std::string, std::string>::const_iterator it = _RP.getRequestH().begin();
	it != _RP.getRequestH().end(); ++it)
	{
		tmp = it->first;
		for (size_t i = 0; i < tmp.length(); i++)
			tmp[i] = std::tolower(tmp[i]);
		_env["HTTP_" + tmp] = it->second;
	}
	// PRINT ENV
	// for(std::unordered_map<std::string, std::string>::const_iterator it = _env.begin();
	// it != _env.end(); ++it)
	// {
	// 	std::cout << PINK << it->first << it->second << RESET << std::endl;
	// }

	char ** env_str = (char **)calloc(sizeof(char *), _env.size() + 1);
	size_t index = 0;

	for (std::unordered_map<std::string, std::string>::const_iterator it = _env.begin();
	it != _env.end(); ++it)
	{
		std::string str = it->first + "=" + it->second;
		char * c_str = (char *)calloc(sizeof(char), str.length());
		char * helpStr = const_cast<char *>(str.c_str());
		size_t i = 0;
		while (helpStr[i]) {
			c_str[i] = helpStr[i];
			++i;
		}
		env_str[index] = c_str;
		++index;
	}
	env_str[index] = NULL;

	// for (int i = 0; env_str[i] != '\0'; i++)
	// 	std::cout << env_str[i] << std::endl;

	// process(env_str);
}
void Cgi::process(char ** env_str)
{
	pid_t pid = fork();
	int fin = fileno(_infile);
	int fout = fileno(_tmp);
	int pipe_fd[2];
	char ** input_str;

	input_str = NULL;
	// make a diffrence btw get und post 

	if (pid < 0)
	{
		//set error page to 500
		throw std::runtime_error("Fork Cgi Error"); //temporary - maybe perror
		close(fin);
		close(fout);
		return ;
	}
	if (pid == 0)
	{

	}
	else
	{

	}

	if (execve(_path.c_str(), input_str, env_str) == -1)
		throw std::runtime_error("Cgi execve error"); //temporary for debugging
	free_array(env_str);
	exit(1);
}
