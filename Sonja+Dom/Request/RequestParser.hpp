#ifndef REQUESTPARSER_HPP
# define REQUESTPARSER_HPP

# include <iostream>
# include <string>
# include <unordered_map>
# include <vector>

#define PINK "\033[1;35m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define LB		"\033[1;34m"
#define RED		"\033[1;31m"
#define RESET  "\033[0m"

#define BLACK "\033[0;30m"
#define CYAN "\033[0;36m"
#define WHITE "\033[0;37m" 



class RequestParser
{

	public:

		RequestParser();
		RequestParser(std::vector<unsigned char> &request);
		// RequestParser( RequestParser const & src );
		~RequestParser();

		// RequestParser &		operator=( RequestParser const & rhs );
		std::string const & getMethod() const;
		std::string const & getURI() const;
		std::string const & getVersion() const;
		uint32_t const & getPort() const;
		// std::size_t const & getLineCount() const;
		//not sure if needed
		std::unordered_map<std::string, std::string> const & getRequestH() const;
		std::vector<std::string> const & getCRLF_split() const;

		void		split_CRLF(std::vector<unsigned char> buffer);
		void		parseRequestLine(std::string reqLine);
		std::string &		RequestLineMethod(std::string & method);
		std::string &		RequestLineURI(std::string & URI);
		void		RequestLineVersion(std::string & version);
		void setPort();



		void		parseRequestHeader();

	private:

		std::string	_method;
		std::string	_URI;
		uint32_t	_port;
		//maybe not const, maybe needs to be modified?
		std::string	_version;
		std::vector<std::string> _CRLF_split;
		std::unordered_map<std::string, std::string> _requestH;
		// std::size_t				_line_count = _requestH.size();

};

std::ostream &			operator<<( std::ostream & o, RequestParser const & i );


#endif /* *************************************************** REQUESTPARSER_H */