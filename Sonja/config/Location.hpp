#ifndef HTTP_Location_HPP
#define HTTP_Location_HPP

#include <functional>
#include <ostream>
#include <memory>
#include <stdint.h>
#include <string>

#include "Server.hpp"

// ************************************************************************** //
//                               Location Class                                //
// ************************************************************************** //

class Location
{
	public:
		Location();
		Location(std::string path);
		~Location();

		std::string							proxy;
		std::string							root;
		std::set<std::string>				allowed_methods;
		std::string							index;
		bool								directory_listing;
	
	//operation overload schreiben zum printen der Daten
};

#endif /* HTTP_Location_HPP */