#ifndef REQUESTHANDLER_HPP
# define REQUESTHANDLER_HPP

# include "RequestParser.hpp"

class RequestHandler: public RequestParser
{

	public:

		RequestHandler();
		RequestHandler( RequestHandler const & src );
		~RequestHandler();

		RequestHandler &		operator=( RequestHandler const & rhs );

	private:

};

std::ostream &			operator<<( std::ostream & o, RequestHandler const & i );

#endif /* ************************************************** REQUESTHANDLER_H */