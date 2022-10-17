
#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <iostream>
#include <algorithm>
#include <sstream>

#include <vector>
#include <set>
#include <map>

#define SUCCESS 0
#define FAILURE 1
#define STREND std::string::npos

// ************************************************************************** //
//                               Location Struct                              //
// ************************************************************************** //

struct Location 
{
	std::string						_proxy;
	std::vector<std::string>		_loc_methods;
	std::string						_index;
	std::string						_root;
	bool							_dir_list;
	void							setProxy(std::string proxy);
	int								setLocMethods(std::string methods);
	int								setIndex(std::string idnex);
	int								setRoot(std::string root);
	int								setDirectoryListing(std::string listing);
	const std::string				&getProxy() const;
	const std::vector<std::string>	&getLocMethods() const;
	const std::string				&getIndex() const;
	const std::string				&getRoot() const;
	bool							getDirectoryListing() const;

	bool		i;
	bool		am;
	bool		r;
	bool		dl;
};

// ************************************************************************** //
//                               Server Class                                 //
// ************************************************************************** //

class Server
{
	private:
		std::string					_server_name;
		std::string					_ip_address;
		uint32_t					_port;
		std::string					_root;
		std::string					_index;
		std::vector<std::string>	_allowed_methods;
		uint32_t					_limit_body;
		std::vector<Location>		_locations;

	public:
		Server();
		~Server();

		bool		sn;
		bool		ip;
		bool		p;
		bool		r;
		bool		i;
		bool		am;
		bool		lb;
		bool		l;
		
		bool		open_bracket;

		const std::vector<Location>		&getLocation() const;
		const std::string				&getServerName() const;
		const std::string				&getIpAddress() const;
		uint32_t						getPort() const;
		const std::string				&getRoot() const;
		const std::string				&getIndex() const;
		const std::vector<std::string>	&getMethods() const;
		uint32_t						getLimitBody() const;

		void						setLocation(std::vector<Location> location);
		void						setServerName(std::string server_name);
		int							setIpAddress(std::string ip_address);
		int							setPort(std::string port);
		int							setRoot(std::string root);
		int							setIndex(std::string idnex);
		int							setMethods(std::string methods);
		int							setLimitBody(std::string methods);
	
		void						reserve_location();
};

#endif /* SERVER_HPP */