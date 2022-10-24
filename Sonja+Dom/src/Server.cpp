#include "Server.hpp"

Server::Server()
{
	_index = "index.html";
	_limit_body = 10000000;
	_port = 0;
	sn = false;
	ip = false;
	p = false;
	r = false;
	i = false;
	am = false;
	lb = false;
	l = false;
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

size_t Server::getNBytes()
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
	limit.erase(remove(limit.begin(), limit.end(), ';'));
	std::stringstream s(limit);
	s >> tmp;
	if (tmp < 0 || check_uint32(limit))
		return (FAILURE);
	this->_limit_body = atol(limit.c_str());
	return (SUCCESS);
}

int Server::setF_Content()
{
	std::ostringstream ss;
	std::ifstream input_file;
	input_file.open(_root + _index);
	std::cout << "/////" << _root + _index << std::endl;
	if (!input_file.is_open())
		return (FAILURE);
	ss << input_file.rdbuf();
	this->_f_content = ss.str();
	std::cout << ">>>>> yoyoyo" << _f_content.size() << std::endl;
	return (SUCCESS);
}

int Server::setImg_Content()
{
	std::ostringstream ss;
	std::ifstream input_img;
	input_img.open(_root + "cat.jpeg");
	std::cout << "/////" << _root + "cat.jpeg" << std::endl;
	if (!input_img.is_open())
		return (FAILURE);
	ss << input_img.rdbuf();
	this->_img_content = ss.str();
	return (SUCCESS);
}

int Server::setFavi_Content()
{
	std::ostringstream ss;
	std::ifstream input_img;
	input_img.open(_root + "favicon.ico");
	std::cout << "/////" << _root + "favicon.ico" << std::endl;
	if (!input_img.is_open())
		return (FAILURE);
	ss << input_img.rdbuf();
	this->_favi_content = ss.str();
	std::cout << "this.>>>>>>" << _favi_content.size() << std::endl;
	return (SUCCESS);
}

int Server::setResponse()
{
	std::string response;
	response = "HTTP/1.1 200 OK\nDate: Thu. 20 May 2004 21:12:58 GMT\nConnection: close\nServer: Apache/1.3.27\nContent-Type: text/html\nContent-Length: 151\n\r\n";
	response.append(getF_Content());
	this->_response = response;	
	return (SUCCESS);
}

int Server::setImg_Response()
{
	std::string response;
	response = "HTTP/1.1 200 OK\nDate: Thu. 20 May 2004 21:12:58 GMT\nConnection: close\nServer: Apache/1.3.27\nContent-Type: image/jpeg\nContent-Length: 29036\n\r\n";
	response.append(getImg_Content());
	this->_imgresponse = response;
	return (SUCCESS);
}

int Server::setFavi_Response()
{
	std::string response;
	response = "HTTP/1.1 200 OK\nDate: Thu. 20 May 2004 21:12:58 GMT\nConnection: close\nServer: Apache/1.3.27\nContent-Type: image/x-icon\nContent-Length: 318\n\r\n";
	response.append(getFavi_Content());
	this->_faviresponse = response;
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

const std::string &Server::getF_Content() const
{
	return (this->_f_content);
}

const std::string &Server::getImg_Content() const
{
	return (this->_img_content);
}

const std::string &Server::getFavi_Content() const
{
	return (this->_favi_content);
}

const std::string &Server::getResponse() const
{
	return (this->_response);
}

const std::string &Server::getImg_Response() const
{
	return (this->_imgresponse);
}

const std::string &Server::getFavi_Response() const
{
	return (this->_faviresponse);
}

int const& Server::getSockFd() const
{
	return (_sock.getSocketFd());
}

const std::vector<Location>	&Server::getLocation() const { return _locations; }

//----------------------Location----------------------//

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
