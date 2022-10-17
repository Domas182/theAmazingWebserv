#ifndef POLLER_HPP
#define POLLER_HPP
#include <sys/socket.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <cstdlib>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <poll.h>
#include <fstream>
#include <sstream>
#include <sys/wait.h>
#include <vector>

class	PollFd
{
		private:
				std::vector<struct pollfd>      _pfd;
		public:
				PollFd();
				~PollFd();
				void	                        addFd(int fd);
				void		                    deleteFd(int i);
				std::vector<struct pollfd>&	    getPfd();
				size_t							getFdCount() const;
};

#endif