#include "../incl/RequestParser.hpp"
#include "../incl/Client.hpp"

extern int	g_error;

RequestParser::RequestParser(){}

RequestParser::~RequestParser(){}

RequestParser::RequestParser(std::vector<unsigned char>& request)
{
	this->_oldLocation = "";
	split_CRLF(request);
}

std::ostream &		operator<<( std::ostream & o, RequestParser const & r )
{
	int i = 0;
	o << GREEN  "Method = " << r.getMethod() << "\n"
	<< "URI = " << r.getURI() << "\n"
	<< "Version = " << r.getVersion() << RESET "\n";
	for (std::vector<std::string>::const_iterator vit = r.getCRLF_split().begin();
	vit != r.getCRLF_split().end(); vit++)
	{
		o << PINK << "VECTOR	" << r.getCRLF_split()[i] << RESET << std::endl;
		i++;
	}
	for(std::unordered_map<std::string, std::string>::const_iterator itnew = r.getRequestH().begin();
	itnew != r.getRequestH().end(); ++itnew)
	{
		o << YELLOW << "MAP	" << itnew->first << " " << itnew->second << RESET << "\n";
	}
	return o;
}

void	RequestParser::split_CRLF(std::vector<unsigned char>& buffer)
{
	std::string CRLF(buffer.begin(), buffer.end());
	std::string delimeter = "\r\n";
	size_t pos = 0;
	while ((pos = CRLF.find(delimeter)) != std::string::npos)
	{
		std::string test = CRLF.substr(0, pos);
		if (!test.empty())
			this->_CRLF_split.push_back(test);
		CRLF.erase(0, pos + delimeter.length());
	}
	if (_CRLF_split.empty())
	{
		g_error = 400;
		throw std::runtime_error("We know what you try here\nOur Webserv is puuuuuurrrrfect though!");
	}
	parseRequestLine(_CRLF_split.front());
	parseRequestHeader();
}

std::string &		RequestParser::RequestLineMethod(std::string &Method)
{
	std::string delimeter = " ";
	size_t pos = 0;
	pos = Method.find(delimeter);
	if (pos != std::string::npos)
	{
		this->_method = Method.substr(0, pos);
		if (this->_method.compare("GET") != 0 
		&& this->_method.compare("POST") != 0
		&& this->_method.compare("DELETE") != 0)
			g_error = 501;
		Method.erase(0, pos + delimeter.length());
		return (Method);
	}
	else
	{
		g_error = 400;
		throw std::runtime_error("Wrong requestline syntax");
	}
	return (Method);
}


std::string &	RequestParser::RequestLineURI(std::string &URI)
{
	std::string delimeter = " ";
	size_t pos = 0;
	pos = URI.find(delimeter);
	if (pos != std::string::npos)
	{
		this->_URI = URI.substr(0, pos);
		URI.erase(0, pos + delimeter.length());
		return (URI);
	}
	else
	{
		g_error = 400;
		throw std::runtime_error("Wrong URI");
	}
}

void	RequestParser::RequestLineVersion(std::string &version)
{
	std::string delimeter = " ";
	size_t pos = 0;
	pos = version.find(delimeter);
	if (pos == std::string::npos)
	{
		std::string test_version = version.substr(0, pos);
		if (test_version.compare("HTTP/1.1") == 0)
			this->_version = version.substr(0, pos);
		else
		{
			g_error = 505;
			throw std::runtime_error("HTTP Version Not Supported");
		}
	}
	else
	{
		g_error = 400;
		throw std::runtime_error("Wrong URI");
	}
}

void	RequestParser::parseRequestLine(std::string reqLine)
{
	RequestLineVersion(RequestLineURI(RequestLineMethod(reqLine)));
}

void	RequestParser::parseRequestHeader()
{

	std::vector<std::string>::iterator it = _CRLF_split.begin();
	it++;
	std::string delimeter = ":";
	for(; it != _CRLF_split.end(); it++)
	{
		size_t pos = 0;
		pos = it->find(delimeter);
		if (pos != std::string::npos)
		{
			std::string a = it->substr(0, pos);
			std::string b = it->substr(pos + 1, it->length());
			this->_requestH.insert(std::pair<std::string, std::string>(a, b));
			a.clear();
			b.clear();
		}
		else
		{
			g_error = 400;
			throw std::runtime_error("Wrong requestheaderfields synatx");
		}
	}
	std::unordered_map<std::string, std::string>::const_iterator got = this->_requestH.find("Referer");
	std::unordered_map<std::string, std::string>::const_iterator endit = this->_requestH.end();
	if (got != endit)
		this->_oldLocation = got->second;
	setPort();
}

void RequestParser::setPort()
{
	std::string tmp;
	std::unordered_map<std::string, std::string>::iterator start = _requestH.find("Host");
	if (start != _requestH.end())
	{
		tmp = start->second;
		size_t i = tmp.find(':');
		tmp = tmp.substr(i + 1);
	}
	this->_port = atol(tmp.c_str());
}

std::string const & RequestParser::getMethod() const
{
	return this->_method;
}

std::string const & RequestParser::getURI() const
{
	return this->_URI;
}

std::string const & RequestParser::getVersion() const
{
	return this->_version;
}

std::unordered_map<std::string, std::string> const & RequestParser::getRequestH() const
{
	return this->_requestH;
}

std::vector<std::string> const & RequestParser::getCRLF_split() const
{
	return this->_CRLF_split;
}

uint32_t const & RequestParser::getPort() const
{
	return this->_port;
}

std::string const & RequestParser::getOldLocation() const
{
	return this->_oldLocation;
}
