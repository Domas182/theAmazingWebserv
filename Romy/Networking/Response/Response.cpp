#include "Response.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Response::Response()
{
	setVersion("HTTP/1.1");
	setCode("200");
	setPhrase("OK");
	setResponseH();
	//setPayload takes a parameter the function that reads the file and gives back a string
	setPayload("setPayload takes as parameter the function that reads the file and gives back a string");

	std::cout << CYAN << this->constructResponse() << RESET << std::endl;
}

// Response::Response( const Response & src )
// {
// }


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Response::~Response()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

// Response &				Response::operator=( Response const & rhs )
// {
// 	//if ( this != &rhs )
// 	//{
// 		//this->_value = rhs.getValue();
// 	//}
// 	return *this;
// }

std::ostream &			operator<<( std::ostream & o, Response const & i )
{
	o << "Response = " << i.constructResponse();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/
std::string Response::constructResponse() const
{
	std::string response = getVersion() + " " + getCode() + " " + getPhrase() + "\n";
	for (std::unordered_map<std::string, std::string>::const_iterator cit = getResponseH().begin() ; cit != getResponseH().end(); ++cit)
	{
		response += cit->first + " " + cit->second + "\n";
	}
	response += "\n";
	response += getPayload();
	return (response);
}

std::string const & Response::getVersion() const
{
	return this->_version;
}

std::string const & Response::getCode() const
{
	return this->_code;
}

std::string const & Response::getPhrase() const
{
	return this->_phrase;
}

std::unordered_map<std::string, std::string> const & Response::getResponseH() const
{
	return this->_responseH;
}

std::string const & Response::getPayload() const
{
	return this->_payload;
}

void				Response::setVersion(std::string const & version)
{
	this->_version = version;
}

void				Response::setCode(std::string const & statusCode)
{
	this->_code = statusCode;
}
//code class
void				Response::setPhrase(std::string const & reasonPhrase)
{
	this->_phrase = reasonPhrase;
}
//phrase class
void				Response::setResponseH()
{
	this->_responseH.insert(std::pair<std::string, std::string>("Date:", " Thu. 20 May 2004 21:12:58 GMT"));
	this->_responseH.insert(std::pair<std::string, std::string>("Connection:", " close"));
	this->_responseH.insert(std::pair<std::string, std::string>("Server:", "  Apache/1.3.27"));
	this->_responseH.insert(std::pair<std::string, std::string>("Content-Type:", "  text/html"));
	this->_responseH.insert(std::pair<std::string, std::string>("Content-Length:", " 151"));
	//the keyvalue won't be hardcoded but received by a function
	//e.g. "Date:", getTimeDate()

}

void				Response::setPayload(std::string readFile)
//readFile is the return of std::string readFileIntoString2(const std::string& path) 
{
	this->_payload = readFile;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */