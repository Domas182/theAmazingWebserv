
#ifndef POLLER_HPP
#define POLLER_HPP

#include "CrazyCats.hpp"

// ************************************************************************** //
//                               PollFd Class                                 //
// ************************************************************************** //

class	PollFd
{
		private:
				std::vector<struct pollfd>		_pfd;
		public:
				PollFd();
				~PollFd();
				void							addFd(int fd);
				void							deleteFd(int i);
				std::vector<struct pollfd>&		getPfd();
				size_t							getFdCount() const;
};

#endif /* POLLER_HPP */