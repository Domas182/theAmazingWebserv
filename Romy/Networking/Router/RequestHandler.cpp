#include "RequestHandler.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

RequestHandler::RequestHandler()
{
}

RequestHandler::RequestHandler( const RequestHandler & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

RequestHandler::~RequestHandler()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

RequestHandler &				RequestHandler::operator=( RequestHandler const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, RequestHandler const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */