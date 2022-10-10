#include "RequestParser.hpp"



/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

RequestParser::RequestParser()
{
}

// RequestParser::RequestParser( const RequestParser & src )
// {
// }


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

RequestParser::~RequestParser()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

// RequestParser &				RequestParser::operator=( RequestParser const & rhs )
// {
// 	//if ( this != &rhs )
// 	//{
// 		//this->_value = rhs.getValue();
// 	//}
// 	return *this;
// }

std::ostream &			operator<<( std::ostream & o, RequestParser const & r )
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
    	std::cout << YELLOW << "MAP	" << itnew->first << " " << itnew->second << RESET << "\n";
	}
	return o;
}
//debugging - method for printing if request was correctly parsed into the requestClassInstance


/*
** --------------------------------- METHODS ----------------------------------
*/

void		RequestParser::split_CRLF(char * buffer)
{

	//TODO:wird der v immer neuen speicher allocieren? bzw wie kann ich am Anfang reserven??
	std::string CRLF = buffer;
	std::string delimeter = "\r\n";
	size_t pos = 0;
	while ((pos = CRLF.find(delimeter)) != std::string::npos)
	{
		std::string test = CRLF.substr(0, pos);
		if (!test.empty())
		{
			this->_CRLF_split.push_back(test);
		}
		CRLF.erase(0, pos + delimeter.length());
	}
	if (_CRLF_split.empty())
		throw std::runtime_error("Request syntax error");
	//catch in the main
	parseRequestLine(_CRLF_split.front());
	parseRequestHeader();
	//body starts here
}

std::string &		RequestParser::RequestLineMethod(std::string &Method)
{
	std::string delimeter = " ";
	size_t pos = 0;

	pos = Method.find(delimeter);
	if (pos != std::string::npos)
	{
		this->_method = Method.substr(0, pos);
		if (this->_method.compare("GET") == 0)
			std::cout << RED << "WORKED" << RESET << std::endl;
		else
			throw std::runtime_error("Wrong Method");
		Method.erase(0, pos + delimeter.length());
		return (Method);
		// pos = Method.find(delimeter);
	}
	throw std::runtime_error("Wrong Method");
	//what should we do if one request is bad? 
	//still keep on with the others or stop the programm?
	
}


std::string &		RequestParser::RequestLineURI(std::string &URI)
{
	std::string delimeter = " ";
	size_t pos = 0;
	pos = URI.find(delimeter);
	if (pos != std::string::npos)
	{
		this->_URI = URI.substr(0, pos);
		//better parsing aka check URI syntax (allowed chars and just *)
		URI.erase(0, pos + delimeter.length());
		return (URI);
	}
	else
		throw std::runtime_error("Wrong Method");
}

void		RequestParser::RequestLineVersion(std::string &version)
{
	std::string delimeter = " ";
	size_t pos = 0;
	pos = version.find(delimeter);
	if (pos == std::string::npos)
		this->_version = version.substr(0, pos);
	else
		throw std::runtime_error("RequestLine parsing failed");
	//TODO:handle return values!
	

}

void		RequestParser::parseRequestLine(std::string reqLine)
{
	//make it more readable
	RequestLineVersion(RequestLineURI(RequestLineMethod(reqLine)));
}

void		RequestParser::parseRequestHeader()
{

	std::vector<std::string>::iterator it = _CRLF_split.begin();
	it++;
	std::string delimeter = ":";
	//hier z.b. kann der User request ohne doppelpunkt eingeben?
	//und sollte man deswegen beim : splitten?
	for(; it != _CRLF_split.end(); it++)
	{
		size_t pos = 0;
		pos = it->find(delimeter);
		if (pos != std::string::npos)
		{
			std::string a = it->substr(0, pos);
			std::string b = it->substr(pos + 1, it->length());
			this->_requestH.insert(std::pair<std::string, std::string>(a, b));
			//check if insert was successful
			a.clear();
			b.clear();
		}
		else
			throw std::runtime_error("RequestHeader parsing failed");
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

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


/* ************************************************************************** */

/*
#include <map>
#include <string>
#include <sstream>
#include <iostream>

std::map<std::string, std::string> mappify1(std::string const& s)
{
    std::map<std::string, std::string> m;

    std::string key, val;
    std::istringstream iss(s);

    while(std::getline(std::getline(iss, key, ':') >> std::ws, val))
        m[key] = val;

    return m;
}
*/