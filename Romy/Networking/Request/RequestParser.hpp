#ifndef REQUESTPARSER_HPP
# define REQUESTPARSER_HPP

# include <iostream>
# include <string>
# include "../Servers/TestServer.hpp"
# include <unordered_map>
# include <vector>



class RequestParser
{

	public:

		RequestParser();
		// RequestParser( RequestParser const & src );
		~RequestParser();

		// RequestParser &		operator=( RequestParser const & rhs );
		std::string const & getMethod() const;
		std::string const & getURI() const;
		std::string const & getVersion() const;
		// std::size_t const & getLineCount() const;
		//not sure if needed
		std::unordered_map<std::string, std::string> const & getRequestH() const;
		std::vector<std::string> const & getCRLF_split() const;

		void		split_CRLF(char * buffer);
		void		parseRequestLine(std::string reqLine);
		std::string &		RequestLineMethod(std::string & method);
		std::string &		RequestLineURI(std::string & URI);
		void		RequestLineVersion(std::string & version);


		void		parseRequestHeader();

	private:

		std::string	_method;
		std::string	_URI;
		//maybe not const, maybe needs to be modified?
		std::string	_version;
		std::vector<std::string> _CRLF_split;
		std::unordered_map<std::string, std::string> _requestH;
		// std::size_t				_line_count = _requestH.size();

};

std::ostream &			operator<<( std::ostream & o, RequestParser const & i );


#endif /* *************************************************** REQUESTPARSER_H */