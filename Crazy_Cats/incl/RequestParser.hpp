#ifndef REQUESTPARSER_HPP
# define REQUESTPARSER_HPP

#include "CrazyCats.hpp"

// ************************************************************************** //
//                               RequestParser Class                          //
// ************************************************************************** //

class RequestParser
{

	public:

		RequestParser();
		RequestParser(std::vector<unsigned char> &request);
		~RequestParser();
		std::string const & 									getMethod() const;
		std::string const & 									getURI() const;
		std::string const & 									getVersion() const;
		uint32_t const & 										getPort() const;
		std::string const & 									getOldLocation() const;
		std::unordered_map<std::string, std::string> const & 	getRequestH() const;
		std::vector<std::string> const & 						getCRLF_split() const;
		void													split_CRLF(std::vector<unsigned char>& buffer);
		void													parseRequestLine(std::string reqLine);
		std::string &											RequestLineMethod(std::string & method);
		std::string &											RequestLineURI(std::string & URI);
		void													RequestLineVersion(std::string & version);
		void 													setPort();
		void													parseRequestHeader();

	private:

		std::string												_method;
		std::string												_URI;
		uint32_t												_port;
		std::string												_version;
		std::string												_oldLocation;
		std::vector<std::string> 								_CRLF_split;
		std::unordered_map<std::string, std::string> 			_requestH;
};

std::ostream &			operator<<( std::ostream & o, RequestParser const & i );

#endif /* REQUESTPARSER_H */