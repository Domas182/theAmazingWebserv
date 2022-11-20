
#ifndef SERVER_HPP
#define SERVER_HPP
#include "CrazyCats.hpp"

#include "Socket.hpp"
#include "Poller.hpp"
#include "Client.hpp"

#define SUCCESS 0
#define FAILURE 1
#define MAX_BODY 65536

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

	bool							i;
	bool							am;
	bool							r;
	bool							dl;
};

// ************************************************************************** //
//                               Server Class                                 //
// ************************************************************************** //

class Server
{
	private:
		std::string					_server_name;
		std::string					_content;
		std::string					_f_content;
		std::string					_img_content;
		std::string					_favi_content;
		uint32_t					_port;
		std::string					_port_str;
		std::string					_root;
		std::string					_index;
		std::vector<std::string>	_allowed_methods;
		std::vector<std::string>	_cgi;
		uint32_t					_limit_body;
		std::string					_error_pages;
		std::vector<Location>		_locations;
		std::string					_response;
		std::string					_imgresponse;
		std::string					_faviresponse;
		ft_tsocket					_sock;

	public:
		Server();
		~Server();

		bool							sn;
		bool							p;
		bool							r;
		bool							i;
		bool							am;
		bool							lb;
		bool							l;
		bool							c;
		bool							ep;
		bool							open_bracket;
		void							continueSetup(std::vector<Server> servs, int index);
		bool							portAvailabe(std::vector<Server> servs, uint32_t port, int i);
		int								sockAccept();
		std::vector<unsigned char>		sockRecv(int i, PollFd &oPoll);
		std::vector<unsigned char>		testRecv(int i, PollFd &oPoll);
		size_t							&getNBytes();
		void							sockSend(int fd, Client& client);
		void							bindPort();
		const std::vector<Location>		&getLocation() const;
		const std::string				&getServerName() const;
		uint32_t						getPort() const;
		const std::string				&getPortStr() const;
		const std::string				&getRoot() const;
		const std::string				&getIndex() const;
		const std::vector<std::string>	&getMethods() const;
		uint32_t						getLimitBody() const;
		const std::string				&getContent() const;
		const std::string				&getResponse() const;
		const std::string				&getErrorPages() const;
		const std::vector<std::string>	&getCgi() const;
		int const						&getSockFd() const;
		void							setLocation(std::vector<Location> location);
		void							setServerName(std::string server_name);
		int								setPort(std::string port);
		int								setRoot(std::string root);
		int								setIndex(std::string idnex);
		int								setMethods(std::string methods);
		int								setLimitBody(std::string limit);
		std::string						set_Content(std::string path, int check);
		int								setErrorPages(std::string error_pages);
		int								setCgi(std::string cgi);
};

#endif /* SERVER_HPP */