#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "CrazyCats.hpp"
#include "Server.hpp"

#define SSTR( x ) static_cast< std::ostringstream & >( ( std::ostringstream() << std::dec << x ) ).str()

// ************************************************************************** //
//                               Response Class                                 //
// ************************************************************************** //

class Response
{

	public:

		Response();

		~Response();

		std::string const & 									getVersion() const;
		std::string const 										getCode() const;
		std::string const & 									getPhrase() const;
		std::unordered_map<std::string, std::string> const & 	getResponseH() const;
		std::string const & 									getPayload() const;

		std::string 											createResponse(int code, Server & server, std::string & path, std::string & version);
		std::string 											createErrorResponse (int code, Server & server);
		std::string												set_time();
		std::string												setContentLength(Server & server);
		std::string												setErrorContentLength();
		std::string												setContentType(std::string & path);
		std::string 											constructResponse() const;
		void													setVersion(std::string const & version);
		void													setCodePhrase(int code);
		void													setResponseH( Server & server,  std::string & path);
		void													setErrorResponseH(Server & server);
		void													setPayload(std::string readFile);
		void													setErrorPayload(int code);
		void													createCodePhraseMap();

	private:

		std::string										_version;
		int												_code;
		std::string										_phrase;
		std::string 									_contentType;
		std::unordered_map<std::string, std::string> 	_responseH;
		std::unordered_map<int, std::string> 			_codePhrase;
		std::string 									_payload;

};

#endif /* RESPONSE_H */