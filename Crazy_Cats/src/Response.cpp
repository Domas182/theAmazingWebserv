#include "../incl/Response.hpp"

Response::Response(){}

Response::~Response(){}

std::string Response::createResponse (int code, Server & server, std::string & path, std::string & version)
{
	createCodePhraseMap();
	setVersion(version);
	setCodePhrase(code);
	setResponseH(server, path);
	setPayload(server.getContent());
	return(this->constructResponse());
}

std::string Response::createErrorResponse (int code, Server & server)
{
	createCodePhraseMap();
	setCodePhrase(code);
	setErrorPayload(code);
	setErrorResponseH(server);
	std::string response = "HTTP/1.1";
	response +=  " " + getCode() + " " + getPhrase() + "\n";
	for (std::unordered_map<std::string, std::string>::const_iterator cit = getResponseH().begin() ; cit != getResponseH().end(); ++cit)
		response += cit->first + "\t" + cit->second + "\n";
	response += getPayload();
	return(response);
}

std::string Response::constructResponse() const
{
	std::string response = getVersion() + " " + getCode() + " " + getPhrase() + "\n";
	for (std::unordered_map<std::string, std::string>::const_iterator cit = getResponseH().begin() ; cit != getResponseH().end(); ++cit)
		response += cit->first + "\t" + cit->second + "\n";
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

void			Response::setCodePhrase(int code)
{
	if (_codePhrase.count(code))
	{
		this->_code = code;
		this->_phrase = _codePhrase.at(code);
	}
	
}

void	Response::setPayload(std::string readFile)
{
	this->_payload = readFile;
}

void	Response::setErrorPayload(int code)
{
	std::ostringstream ss;
	std::ifstream input_file;
	if (code == 400)
		input_file.open("docs/error_pages/400.html");
	else if (code == 405)
		input_file.open("docs/error_pages/405.html");
	else if (code == 504)
		input_file.open("docs/error_pages/504.html");
	ss << input_file.rdbuf();
	setPayload(ss.str());
}

std::string	Response::setContentLength(Server & server)
{
	uint32_t len = server.getContent().length();
	std::stringstream ss;
	ss << len;
	std::string content_len = ss.str();
	return (content_len);
}

std::string	Response::setErrorContentLength()
{
	uint32_t len = this->_payload.length();
	std::stringstream ss;
	ss << len;
	std::string content_len = ss.str();
	return (content_len);
}

std::string Response::set_time()
{
    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
	std::string s(std::ctime(&end_time));
	return (s);
}

std::string Response::setContentType(std::string & path)
{
	std::string content_type = path.substr(path.find(".") + 1);
	return (content_type);
}

void	Response::setResponseH(Server & server, std::string & path)
{
	this->_responseH.insert(std::pair<std::string, std::string>("Date:", this->set_time()));
	this->_responseH.insert(std::pair<std::string, std::string>("Content-Length:", this->setContentLength(server)));
	this->_responseH.insert(std::pair<std::string, std::string>("Content-Type:", this->setContentType(path)));
	this->_responseH.insert(std::pair<std::string, std::string>("Server:", server.getServerName()));
}

void	Response::setErrorResponseH(Server & server)
{
	this->_responseH.insert(std::pair<std::string, std::string>("Date:", this->set_time()));
	this->_responseH.insert(std::pair<std::string, std::string>("Content-Length:", this->setErrorContentLength()));
	this->_responseH.insert(std::pair<std::string, std::string>("Content-Type:", "html"));
	this->_responseH.insert(std::pair<std::string, std::string>("Server:", server.getServerName()));
}


void	Response::createCodePhraseMap()
{

		this->_codePhrase.insert(std::pair<int, std::string>(100,"Continue"));
		this->_codePhrase.insert(std::pair<int, std::string>(101,"Switching Protocols"));
		this->_codePhrase.insert(std::pair<int, std::string>(200,"OK"));
		this->_codePhrase.insert(std::pair<int, std::string>(201,"Created"));
		this->_codePhrase.insert(std::pair<int, std::string>(202,"Accepted"));
		this->_codePhrase.insert(std::pair<int, std::string>(203,"Nonauthoritative Information"));
		this->_codePhrase.insert(std::pair<int, std::string>(204,"No Content"));
		this->_codePhrase.insert(std::pair<int, std::string>(205,"Reset Content"));
		this->_codePhrase.insert(std::pair<int, std::string>(206,"Partial Content"));
		this->_codePhrase.insert(std::pair<int, std::string>(300,"Multiple Choices"));
		this->_codePhrase.insert(std::pair<int, std::string>(301,"Moved Permanently"));
		this->_codePhrase.insert(std::pair<int, std::string>(302,"Found"));
		this->_codePhrase.insert(std::pair<int, std::string>(303,"See Other"));
		this->_codePhrase.insert(std::pair<int, std::string>(304,"Not Modified"));
		this->_codePhrase.insert(std::pair<int, std::string>(305,"Use Proxy"));
		this->_codePhrase.insert(std::pair<int, std::string>(306,"Unused"));
		this->_codePhrase.insert(std::pair<int, std::string>(307,"Temporary Redirect"));
		this->_codePhrase.insert(std::pair<int, std::string>(400,"Bad Request"));
		this->_codePhrase.insert(std::pair<int, std::string>(401,"Unauthorized"));
		this->_codePhrase.insert(std::pair<int, std::string>(402,"Payment Required"));
		this->_codePhrase.insert(std::pair<int, std::string>(403,"Forbidden"));
		this->_codePhrase.insert(std::pair<int, std::string>(404,"Not Found"));
		this->_codePhrase.insert(std::pair<int, std::string>(405,"Method Not Allowed"));
		this->_codePhrase.insert(std::pair<int, std::string>(406,"Not Acceptable"));
		this->_codePhrase.insert(std::pair<int, std::string>(407,"Proxy Authentication Required"));
		this->_codePhrase.insert(std::pair<int, std::string>(408,"Request Timeout"));
		this->_codePhrase.insert(std::pair<int, std::string>(409,"Conflict"));
		this->_codePhrase.insert(std::pair<int, std::string>(410,"Gone"));
		this->_codePhrase.insert(std::pair<int, std::string>(411,"Length Required"));
		this->_codePhrase.insert(std::pair<int, std::string>(412,"Precondition Failed"));
		this->_codePhrase.insert(std::pair<int, std::string>(413,"Request Entity Too Large"));
		this->_codePhrase.insert(std::pair<int, std::string>(414,"Request-url Too Long"));
		this->_codePhrase.insert(std::pair<int, std::string>(415,"Unsupported Media Type"));
		this->_codePhrase.insert(std::pair<int, std::string>(416,"Requested Range Not Satisfiable"));
		this->_codePhrase.insert(std::pair<int, std::string>(417,"Expectation Failed"));
		this->_codePhrase.insert(std::pair<int, std::string>(500,"Internal Server Error"));
		this->_codePhrase.insert(std::pair<int, std::string>(501,"Not Implemented"));
		this->_codePhrase.insert(std::pair<int, std::string>(502,"Bad Gateway"));
		this->_codePhrase.insert(std::pair<int, std::string>(503,"Service Unavailable"));
		this->_codePhrase.insert(std::pair<int, std::string>(504,"Gateway Timeout"));
		this->_codePhrase.insert(std::pair<int, std::string>(505,"HTTP Version Not Supported"));

}