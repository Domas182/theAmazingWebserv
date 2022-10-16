#include "Response.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Response::Response(int code)
{
	createCodePhraseMap();
	setVersion("HTTP/1.1");
	setCodePhrase(code);
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
		response += cit->first + "\t" + cit->second + "\n";
	}
	response += "\n";
	response += getPayload();
	return (response);
}

std::string const & Response::getVersion() const
{
	return this->_version;
}

std::string const Response::getCode() const
{

	return (SSTR(this->_code));
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

void				Response::setResponseH()
{
	this->_responseH.insert(std::pair<std::string, std::string>("Date:", "Thu. 20 May 2004 21:12:58 GMT"));
	this->_responseH.insert(std::pair<std::string, std::string>("Connection:", "close"));
	this->_responseH.insert(std::pair<std::string, std::string>("Server:", "Apache/1.3.27"));
	this->_responseH.insert(std::pair<std::string, std::string>("Content-Type:", "text/html"));
	this->_responseH.insert(std::pair<std::string, std::string>("Content-Length:", "151"));
	//the keyvalue won't be hardcoded but received by a function
	//e.g. "Date:", getTimeDate()
}

void			Response::createCodePhraseMap()
{

		this->_codePhrase.insert(std::pair<int, std::string>(100,": Continue"));
		this->_codePhrase.insert(std::pair<int, std::string>(101,": Switching Protocols"));
		this->_codePhrase.insert(std::pair<int, std::string>(200,": OK"));
		this->_codePhrase.insert(std::pair<int, std::string>(201,": Created"));
		this->_codePhrase.insert(std::pair<int, std::string>(202,": Accepted"));
		this->_codePhrase.insert(std::pair<int, std::string>(203,": Nonauthoritative Information"));
		this->_codePhrase.insert(std::pair<int, std::string>(204,": No Content"));
		this->_codePhrase.insert(std::pair<int, std::string>(205,": Reset Content"));
		this->_codePhrase.insert(std::pair<int, std::string>(206,": Partial Content"));
		this->_codePhrase.insert(std::pair<int, std::string>(300,": Multiple Choices"));
		this->_codePhrase.insert(std::pair<int, std::string>(301,": Moved Permanently"));
		this->_codePhrase.insert(std::pair<int, std::string>(302,": Found"));
		this->_codePhrase.insert(std::pair<int, std::string>(303,": See Other"));
		this->_codePhrase.insert(std::pair<int, std::string>(304,": Not Modified"));
		this->_codePhrase.insert(std::pair<int, std::string>(305,": Use Proxy"));
		this->_codePhrase.insert(std::pair<int, std::string>(306,": Unused"));
		this->_codePhrase.insert(std::pair<int, std::string>(307,": Temporary Redirect"));
		this->_codePhrase.insert(std::pair<int, std::string>(400,": Bad Request"));
		this->_codePhrase.insert(std::pair<int, std::string>(401,": Unauthorized"));
		this->_codePhrase.insert(std::pair<int, std::string>(402,": Payment Required"));
		this->_codePhrase.insert(std::pair<int, std::string>(403,": Forbidden"));
		this->_codePhrase.insert(std::pair<int, std::string>(404,": Not Found"));
		this->_codePhrase.insert(std::pair<int, std::string>(405,": Method Not Allowed"));
		this->_codePhrase.insert(std::pair<int, std::string>(406,": Not Acceptable"));
		this->_codePhrase.insert(std::pair<int, std::string>(407,": Proxy Authentication Required"));
		this->_codePhrase.insert(std::pair<int, std::string>(408,": Request Timeout"));
		this->_codePhrase.insert(std::pair<int, std::string>(409,": Conflict"));
		this->_codePhrase.insert(std::pair<int, std::string>(410,": Gone"));
		this->_codePhrase.insert(std::pair<int, std::string>(411,": Length Required"));
		this->_codePhrase.insert(std::pair<int, std::string>(412,": Precondition Failed"));
		this->_codePhrase.insert(std::pair<int, std::string>(413,": Request Entity Too Large"));
		this->_codePhrase.insert(std::pair<int, std::string>(414,": Request-url Too Long"));
		this->_codePhrase.insert(std::pair<int, std::string>(415,": Unsupported Media Type"));
		this->_codePhrase.insert(std::pair<int, std::string>(416,": Requested Range Not Satisfiable"));
		this->_codePhrase.insert(std::pair<int, std::string>(417,": Expectation Failed"));
		this->_codePhrase.insert(std::pair<int, std::string>(500,": Internal Server Error"));
		this->_codePhrase.insert(std::pair<int, std::string>(501,": Not Implemented"));
		this->_codePhrase.insert(std::pair<int, std::string>(502,": Bad Gateway"));
		this->_codePhrase.insert(std::pair<int, std::string>(503,": Service Unavailable"));
		this->_codePhrase.insert(std::pair<int, std::string>(504,": Gateway Timeout"));
		this->_codePhrase.insert(std::pair<int, std::string>(505,": HTTP Version Not Supported"));

	
}

void			Response::setCodePhrase(int code)
{
	if (_codePhrase.count(code))
	{
		this->_code = code;
		this->_phrase = _codePhrase.at(code);
	}
	
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