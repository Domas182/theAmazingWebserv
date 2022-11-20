#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "CrazyCats.hpp"
#include "Poller.hpp"
#include "Client.hpp"

// ************************************************************************** //
//                               ft_tsocket Class                             //
// ************************************************************************** //

class PollFd;

class Client;

class ft_tsocket
{
	private:
		int							_socketFd;
		struct sockaddr_in			_address;
		int							_addrlen;
		uint16_t					_port;

	public:
		size_t						nbytes;

		ft_tsocket();
		ft_tsocket(uint16_t port);
		~ft_tsocket();
		ft_tsocket	&operator=(ft_tsocket const& src);
		void						setToNonBlocking();
		void						socketBind();
		void						socketListen();
		int							socketAccept();
		void						socketSend(int fd, Client& client);
		std::vector<unsigned char>	socketRecv(int i, PollFd &objectPoll);
		struct sockaddr_in const&	getAddress() const;
		int const& 					getSocketFd() const;
		int 						getAddrlen();
};

#endif /* SOCKET_HPP */