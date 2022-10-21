#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include <string>
# include <unordered_map>
# include <vector>
#include <sstream>
#define PINK "\033[1;35m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define LB		"\033[1;34m"
#define RED		"\033[1;31m"
#define RESET  "\033[0m"

#define BLACK "\033[0;30m"
#define CYAN "\033[0;36m"
#define WHITE "\033[0;37m" 

#define SSTR( x ) static_cast< std::ostringstream & >( ( std::ostringstream() << std::dec << x ) ).str()


class Response
{

	public:

		Response(int code);
		~Response();

		std::string const & getVersion() const;
		std::string const getCode() const;
		std::string const & getPhrase() const;
		std::unordered_map<std::string, std::string> const & getResponseH() const;
		std::string const & getPayload() const;

		void				setVersion(std::string const & version);
		void				setCodePhrase(int code);
		void				setResponseH();
		void				setPayload(std::string readFile);
		//maybe don't need them fi they are in the constructor already;
		void				createCodePhraseMap();
		std::string 		constructResponse() const;

		// Response( Response const & src );
		// Response &		operator=( Response const & rhs );

	private:

		std::string	_version;
		int	_code;
		std::string	_phrase;
		std::unordered_map<std::string, std::string> _responseH;
		std::unordered_map<int, std::string> _codePhrase;
		std::string _payload;


};

std::ostream &			operator<<( std::ostream & o, Response const & i );

#endif /* ******************************************************** RESPONSE_H */