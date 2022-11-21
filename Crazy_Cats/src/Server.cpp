#include "../incl/Server.hpp"

extern int	g_error;

Server::Server()
{
	_index = "index.html";
	_limit_body = MAX_BODY;
	_error_pages = "error_pages/";
	_port = 0;
	sn = false;
	p = false;
	r = false;
	i = false;
	am = false;
	lb = false;
	l = false;
	c = false;
	ep = false;
	open_bracket = true;
	_allowed_methods.push_back("GET");
	_allowed_methods.push_back("POST");
	_allowed_methods.push_back("DELETE");
}

Server::~Server()
{}

void Server::continueSetup(std::vector<Server> servs, int index)
{
	if (portAvailabe(servs, _port, index))
	{
		_sock.socketBind();
		_sock.socketListen();
	}
}

bool Server::portAvailabe(std::vector<Server> servs, uint32_t port, int i)
{
	for (int j = 0; j < i; j++)
		if(servs[j].getPort() == port)
			return (false);
	return (true);
}

int Server::sockAccept()
{
	return (_sock.socketAccept());
}

std::vector<unsigned char> Server::sockRecv(int i, PollFd &oPoll)
{
	return(_sock.socketRecv(i, oPoll));
}

size_t &Server::getNBytes()
{
	return(_sock.nbytes);
}

void Server::bindPort()
{
	_sock = ft_tsocket(_port);
	_sock.setToNonBlocking();
}

void Server::sockSend(int fd, Client& client)
{
	_sock.socketSend(fd, client);
}

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
	if (server_name.find(';') != std::string::npos)
		server_name.erase(remove(server_name.begin(), server_name.end(), ';'));
	this->_server_name = server_name;
}

int Server::setPort(std::string port)
{
	uint32_t tmp;
	if (port.find(';') != std::string::npos)
		port.erase(remove(port.begin(), port.end(), ';'));
	this->_port_str = port;
	std::stringstream s(port);
	s >> tmp;
	if (tmp > 65536 || tmp < 0 || check_uint32(port))
		return (FAILURE);
	this->_port = atol(port.c_str());
	return (SUCCESS);
}

int Server::setRoot(std::string root)
{
	if (root.find(';') != std::string::npos)
		root.erase(remove(root.begin(), root.end(), ';'));
	if (count(root.begin(), root.end(), '/') < 1)
		return (FAILURE);
	this->_root = root;
	return (SUCCESS);
}

int Server::setIndex(std::string index)
{
	if (index.find(';') != std::string::npos)
		index.erase(remove(index.begin(), index.end(), ';'));
	if (count(index.begin(), index.end(), '.') != 1)
		return (FAILURE);
	this->_index = index;
	return (SUCCESS);
}

int Server::setMethods(std::string methods)
{
	if (methods.find(';') != std::string::npos)
		methods.erase(remove(methods.begin(), methods.end(), ';'));
	for (int i = 0; i < 3; i++)
		_allowed_methods.at(i).clear();
	_allowed_methods.clear();
	std::string tmp = methods;
	char *token = strtok(const_cast<char*>(tmp.c_str()), ",");
	char *tmp2 = token;
	char *tmp3 = NULL;
	while (token != NULL)
	{
		if ((strcmp(token, "GET") == 0) || (strcmp(token, "POST") == 0) || (strcmp(token, "DELETE") == 0))
		{
			_allowed_methods.push_back(token);
			token = strtok(NULL, ",");
			if (token)
			{
				if (strcmp(tmp2, token) == 0)
					return (FAILURE);
				if (tmp3)
				{
					if (strcmp(tmp3, token) == 0)
						return (FAILURE);
				}
				tmp3 = token;
			}
		}
		else
			return (FAILURE);
	}
	return (SUCCESS);
}

int Server::setLimitBody(std::string limit)
{
	uint32_t tmp;
	if (limit.find(';') != std::string::npos)
		limit.erase(remove(limit.begin(), limit.end(), ';'));
	std::stringstream s(limit);
	s >> tmp;
	if (tmp < 0 || tmp >= 4294967295)
		return (FAILURE);
	this->_limit_body = atol(limit.c_str());
	return (SUCCESS);
}

int Server::setErrorPages(std::string error_pages)
{
	if (error_pages.find(';') != std::string::npos)
		error_pages.erase(remove(error_pages.begin(), error_pages.end(), ';'));
	if (count(error_pages.begin(), error_pages.end(), '/') < 1)
		return (FAILURE);
	this->_error_pages = error_pages;
	return (SUCCESS);
}

int Server::setCgi(std::string cgi)
{
	if (cgi.find(';') != std::string::npos)
		cgi.erase(remove(cgi.begin(), cgi.end(), ';'));
	std::string tmp = cgi;
	char *token = strtok(const_cast<char*>(tmp.c_str()), ",");
	char *tmp2 = token;
	char *tmp3 = NULL;
	while (token != NULL)
	{
		if ((strcmp(token, "php") == 0) || (strcmp(token, "py") == 0))
		{
			_cgi.push_back(token);
			token = strtok(NULL, ",");
			if (token)
			{
				if (strcmp(tmp2, token) == 0)
					return (FAILURE);
				if (tmp3)
				{
					if (strcmp(tmp3, token) == 0)
						return (FAILURE);
				}
				tmp3 = token;
			}
		}
		else
			return (FAILURE);
	}
	return (SUCCESS);
}


std::string Server::set_Content(std::string path, int check)
{
	std::string str = std::to_string(check);
	if (check != 1)
		path = _root + _error_pages + str + ".html";
	std::ostringstream ss;
	std::ifstream input_file;
	input_file.open(path);
	ss << input_file.rdbuf();
	this->_content = ss.str();
	return (path);
}

const std::string &Server::getServerName() const
{
	return (this->_server_name);
}

uint32_t Server::getPort() const
{
	return (this->_port);
}

const std::string &Server::getPortStr() const
{
	return (this->_port_str);
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

const std::string &Server::getErrorPages() const
{
	return (this->_error_pages);
}

const std::vector<std::string> &Server::getCgi() const
{
	return (this->_cgi);
}

const std::string &Server::getContent() const
{
	return (this->_content);
}


const std::string &Server::getResponse() const
{
	return (this->_response);
}

int const& Server::getSockFd() const
{
	return (_sock.getSocketFd());
}

const std::vector<Location>	&Server::getLocation() const { return _locations; }

//----------------------Location----------------------//

void Location::setProxy(std::string proxy)
{
	if (proxy.find(';') != std::string::npos)
		proxy.erase(remove(proxy.begin(), proxy.end(), ';'));
	_proxy = proxy;
}

int Location::setLocMethods(std::string methods)
{
	if (methods.find(';') != std::string::npos)
		methods.erase(remove(methods.begin(), methods.end(), ';'));
	std::string tmp = methods;
	char *token = strtok(const_cast<char*>(tmp.c_str()), ",");
	char *tmp2 = token;
	char *tmp3 = NULL;
	while (token != NULL)
	{
		if ((strcmp(token, "GET") == 0) || (strcmp(token, "POST") == 0) || (strcmp(token, "DELETE") == 0))
		{
			_loc_methods.push_back(token);
			token = strtok(NULL, ",");
			if (token)
			{
				if (strcmp(tmp2, token) == 0)
					return (FAILURE);
				if (tmp3)
				{
					if (strcmp(tmp3, token) == 0)
						return (FAILURE);
				}
				tmp3 = token;
			}
		}
		else
			return (FAILURE);
	}
	return (SUCCESS);
}

int Location::setIndex(std::string index)
{
	if (index.find(';') != std::string::npos)
		index.erase(remove(index.begin(), index.end(), ';'));
	if (count(index.begin(), index.end(), '.') != 1)
		return (FAILURE);
	this->_index = index;
	return (SUCCESS);
}

int Location::setRoot(std::string root)
{
	if (root.find(';') != std::string::npos)
		root.erase(remove(root.begin(), root.end(), ';'));
	if (count(root.begin(), root.end(), '/') < 1)
		return (FAILURE);
	this->_root = root;
	return (SUCCESS);
}

int Location::setDirectoryListing(std::string listing)
{
	if (listing.find(';') != std::string::npos)
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
