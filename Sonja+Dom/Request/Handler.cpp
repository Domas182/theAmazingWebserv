#include "Handler.hpp"
#include "RequestParser.hpp"

Handler::Handler(RequestParser RP)
{
	std::string	_method = RP.getMethod();
	std::string	_URI = RP.getURI();
	std::string	_version = RP.getVersion();
	std::unordered_map<std::string, std::string> _requestH = RP.getRequestH();
	start_handling();
}

Handler::~Handler()
{}

void	Handler::start_handling()
{
	std::unordered_map<std::string, std::string>::const_iterator got = _requestH.find("Host");
	if (got == _requestH.end())
	{
		//set error page
		throw std::runtime_error("No Host in Request"); //only temporary
	}
	else
	{
		std::cout << "first: " << got->first << "second " << got->second << std::endl;
	}
}