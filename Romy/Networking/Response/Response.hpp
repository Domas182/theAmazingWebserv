#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include <string>
# include "../Servers/TestServer.hpp"
# include <unordered_map>
# include <vector>

class Response
{

	public:

		Response();
		~Response();

		std::string const & getVersion() const;
		std::string const & getCode() const;
		std::string const & getPhrase() const;
		std::unordered_map<std::string, std::string> const & getResponseH() const;
		std::string const & getPayload() const;

		void				setVersion(std::string const & version);
		void				setCode(std::string const & statusCode);
		void				setPhrase(std::string const & reasonPhrase);
		void				setResponseH();
		void				setPayload(std::string readFile);
		//maybe don't need them fi they are in the constructor already;
		std::string constructResponse() const;

		// Response( Response const & src );
		// Response &		operator=( Response const & rhs );

	private:


		std::string	_version;
		std::string	_code;
		std::string	_phrase;
		std::unordered_map<std::string, std::string> _responseH;
		std::string _payload;


};

std::ostream &			operator<<( std::ostream & o, Response const & i );

#endif /* ******************************************************** RESPONSE_H */