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
	//wie kann ich hier abfangen ob auch wirklich CRLF an Ende ist
	//bzw was passiert bei einem fehler??
	//wird der v immer neuen speicher allocieren? bzw wie kann ich am Anfang reserven??
	std::string CRLF = buffer;
	std::string delimeter = "\r\n";
	size_t pos = 0;
	while ((pos = CRLF.find(delimeter)) != std::string::npos)
	{
		this->_CRLF_split.push_back(CRLF.substr(0, pos));
		CRLF.erase(0, pos + delimeter.length());
	}
	//doppel CRLF checken
	parseRequestLine(_CRLF_split.front());
	//abbrechen wenn ein parseteil schief geht!
	parseRequestHeader();
}

void		RequestParser::parseRequestLine(std::string reqLine)
{
	std::string delimeter = " ";
	size_t pos = 0;

	pos = reqLine.find(delimeter);
	this->_method = reqLine.substr(0, pos);
	reqLine.erase(0, pos + delimeter.length());
	pos = reqLine.find(delimeter);
	this->_URI = reqLine.substr(0, pos);
	reqLine.erase(0, pos + delimeter.length());
	pos = reqLine.find(delimeter);
	this->_version = reqLine.substr(0, pos);
	if (pos != std::string::npos)
	{
		std::cout << "syntax error" << std::endl;
		// return EXIT_FAILURE;
		//TODO:handle return values!
	}

}
void		RequestParser::parseRequestHeader()
{

	std::vector<std::string>::iterator it = _CRLF_split.begin();
	it++;
	std::string delimeter = " ";
	for(; it != _CRLF_split.end(); it++)
	{
		size_t pos = 0;
		pos = it->find(delimeter);
		std::string a = it->substr(0, pos);
		std::string b = it->substr(pos + 1, it->length());
		// this->_requestH[a] = b;//other way to insert
		//check if insert geklappt hat
		//
		this->_requestH.insert(std::pair<std::string, std::string>(a, b));
		a.clear();
		b.clear();
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