#include "../incl/Cgi.hpp"

extern int	g_error;

Cgi::Cgi(Server & server, std::string path, std::string query, std::string type, RequestParser & RP, std::string _body_send) : 
	_path(path), _query(query), _type(type), _RP(RP), _body(_body_send)
{
	_method = _RP.getMethod();
	_response.clear();
	_error = false;
	in = tmpfile();
	tmp = tmpfile();
	set_exec_str();
	set_Env(server);
	CgiResponse(server);
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

void Cgi::set_exec_str()
{
	if (_type == "php")
		_exec_str = "docs/cgi/php-cgi";
	else if (_type == "py")
		_exec_str = _path;
	if (_query != "")
	{
			if (_query.at(1) != '&')
			{
				std::string tmp = _query.substr(1);
				_query = "?&" + tmp;
			}
	}
}

void Cgi::set_Env(Server & server)
{
	_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	_env["PATH_INFO"] =_path;
	_env["PATH_TRANSLATED"] = _path;
	_env["REDIRECT_STATUS"] = "200";
	_env["REQUEST_METHOD"] = _method;
	_env["SERVER_NAME"] = server.getServerName();
	_env["SERVER_PORT"] = server.getPortStr();
	_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	if (_method == "GET")
		_env["QUERY_STRING"] = _query;
	std::string tmp;
	for(std::unordered_map<std::string, std::string>::const_iterator it = _RP.getRequestH().begin();
	it != _RP.getRequestH().end(); ++it)
	{
		tmp = it->first;
		for (size_t i = 0; i < tmp.length(); i++)
			tmp[i] = std::toupper(tmp[i]);
		_env["HTTP_" + tmp] = it->second;
	}
	char ** env_str = (char **)calloc(sizeof(char *), _env.size() + 1);
	size_t index = 0;

	for (std::unordered_map<std::string, std::string>::const_iterator it = _env.begin();
	it != _env.end(); ++it)
	{
		std::string str = it->first + "=" + it->second;
		char * c_str = (char *)calloc(sizeof(char), str.length() + 1);
		char * helpStr = const_cast<char *>(str.c_str());
		size_t i = 0;
		while (helpStr[i]) {
			c_str[i] = helpStr[i];
			++i;
		}
		c_str[i] = '\0';
		env_str[index] = c_str;
		++index;
	}
	process(env_str);
}

void Cgi::process(char ** env_str)
{
	int fin = fileno(in);
	int fout = fileno(tmp);
	char ** input_str = NULL;

	if (_method == "POST")
		this->body = const_cast<char *>(_body.c_str());

	pid_t pid = fork();

	if (pid == -1)
	{
		g_error = 500;
		close(fin);
		close(fout);
		return ;
	}

	if (!pid)
	{
		if (dup2(fin, STDIN_FILENO) == -1)
			g_error = 500;
		if (dup2(fout, STDOUT_FILENO) == -1)
			g_error = 500;
		write(fin, _body.c_str(), _body.size());
		if (lseek(fin, 0, SEEK_SET) == -1)
			g_error = 500;
		close(fin);
		close(fout);
		if (execve(_exec_str.c_str(), input_str, env_str) == -1)
		{
			g_error = 500;
			exit(-1);
		}
		exit(1);
	}

	if (pid)
	{
		close(fin);
		free_array(env_str);
		free_array(input_str);
	}
	pid_t	ret = 0;
	int		wait_child;
	int 	child_exit_status;
	while (ret != -1)
	{
		ret = wait(&wait_child);
		if (ret == -1)
			break ;
		else if (ret == pid)
		{
			WIFEXITED(wait_child);
			child_exit_status = WEXITSTATUS(wait_child);
			if (child_exit_status == 255)
				g_error = 500;
		}
	}
}

void	Cgi::CgiResponse(Server & server)
{
	fseek(tmp, 0, SEEK_END);
	long len = ftell(tmp);
	rewind(tmp);
	char *buf = (char*)calloc(len, sizeof(char));
	if (buf == NULL)
		g_error = 500;
	long check = fread(buf, 1, len, tmp);
	if (check != len)
		g_error = 500;
	std::string read = std::string(buf);
	free(buf);
	int fout = fileno(tmp);
	close(fout);
	if (g_error != 200)
	{
		std::string str = std::to_string(g_error);
		std::string path = server.getRoot() + server.getErrorPages() + str + ".html";
		std::ostringstream ss;
		std::ifstream input_file;
		input_file.open(path);
		if (!input_file.is_open())
			g_error = 404;
		ss << input_file.rdbuf();
		this->_response = ss.str();
		_error = true;
	}
	create_Response(server, read);
}

void	Cgi::create_Response(Server & server, std::string read)
{
	if (_error != true)
	{
		size_t start = read.find("\r\n\r\n") + 4;
		std::string	body = read.substr(start, std::string::npos);
		this->_response = "HTTP/1.1 200 OK\r\nContent-Length: ";
		this->_response += std::to_string(body.length());
		this->_response += "\nContent-Type: ";
		this->_response += _type;
		this->_response += "\nLocation: ";
		this->_response += _path;
		this->_response += "\nServer: ";
		this->_response += server.getServerName();
		this->_response += "\nDate: ";
		this->_response += this->set_time();
		this->_response += "\r\n\r\n";
		this->_response += body;
	}
}

std::string const & Cgi::getResponse() const
{
	return (this->_response);
}

bool const & Cgi::getError() const
{
	return (this->_error);
}

std::string Cgi::set_time()
{
	std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);
	std::string s(std::ctime(&end_time));
	return (s);
}