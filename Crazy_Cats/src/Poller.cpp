#include "../incl/Poller.hpp"

PollFd::PollFd() {}

PollFd::~PollFd() {}

void PollFd::addFd(int fd)
{
	struct pollfd pfd;
	pfd.fd = fd;
	//pfd.events = POLLIN | POLLOUT;
	pfd.events = POLLRDNORM | POLLWRNORM;
	_pfd.push_back(pfd);
}

void PollFd::deleteFd(int i)
{
	std::vector<struct pollfd>::iterator it(_pfd.begin());
	for (int j = 0; j < i; j++)
		it++;
	_pfd.erase(it);
}

std::vector<struct pollfd>& PollFd::getPfd()
{
	return (_pfd);
}

size_t  PollFd::getFdCount() const
{
	return (_pfd.size());
}
