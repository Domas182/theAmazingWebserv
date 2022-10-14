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
#define PORT 9999

std::string readFileIntoString2(const std::string& path) {
    std::ostringstream ss;
	std::ifstream input_file(path);
    if (!input_file.is_open()) {
        std::cerr << "Could not open the file - '"
             << path << "'" << std::endl;
        exit(EXIT_FAILURE);
    }
    ss << input_file.rdbuf();
    return ss.str();
}

class	PollFd
{
		private:
				std::vector<struct pollfd> _pfd;
		public:
				PollFd() {}
				//PollFd(PollFd const& src);
				~PollFd() {}

				//PollFd &operator=(PollFd const& src);

				void	addFd(int fd)
				{
					struct pollfd pfd;
					pfd.fd = fd;
					pfd.events = POLLIN | POLLOUT;
					
					_pfd.push_back(pfd);
				}

				void		deleteFd(int i)
				{
					std::vector<struct pollfd>::iterator it(_pfd.begin());
					for (int j = 0; j < i; j++)
						it++;
					_pfd.erase(it);
				}

				std::vector<struct pollfd>&	getPfd()
				{
						return (_pfd);
				}

				size_t							getFdCount() const
				{
						return (_pfd.size());
				}


};

class ft_tsocket
{
	private:
		int			_socketFd;
		struct		sockaddr_in _address;
		int			addrlen;
		uint16_t	_port;

	public:
		size_t			nbytes;

		ft_tsocket() {;}
		~ft_tsocket() {;}
		
		ft_tsocket(uint16_t port) : _port(port)
	{
		nbytes = 0;
		_socketFd = socket(AF_INET, SOCK_STREAM, 0); /*1*/
		_address.sin_family = AF_INET;
		_address.sin_addr.s_addr = INADDR_ANY;
		_address.sin_port = htons(_port);
		memset(_address.sin_zero, '\0', sizeof _address.sin_zero);
		const int trueFlag = 1;
		if (setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &trueFlag, sizeof(int)) < 0)
			perror("Failure");
	}

		ft_tsocket	&operator=(ft_tsocket const& src)
		{
			_socketFd = src._socketFd;
			_address = src._address;
			addrlen = src.addrlen;
			_port = src._port;
			return (*this);
		}

		void	setToNonBlocking()
		{
			fcntl(_socketFd, F_SETFL, O_NONBLOCK); //server socket must be set to non-blocking
		}

		void	socketBind() /*2*/
		{
			if (bind(_socketFd, (struct sockaddr *)&_address, sizeof(_address)) < 0)
			{
				std::cout << "Failed to bind to port. errno: " << errno << std::endl;
				exit(EXIT_FAILURE);
			}
		}

		void	socketListen()
		{	
			listen(_socketFd, 10);
 			std::cout << "Server listening on port: " << _port << std::endl << std::endl;
		}

		int	socketAccept()
		{
			int addrlen = sizeof(_address);
			int new_socket;

	        new_socket = accept(_socketFd, (struct sockaddr *)&_address, (socklen_t *)&addrlen);
			return (new_socket);
		}

		int		socketSend(int fd, std::string answer)
		{
			int sentBytes = 0;

			sentBytes = send(fd, &answer[0], answer.size(), 0);
			return (sentBytes);
		}

		std::vector<unsigned char>	socketRecv(int i, PollFd &objectPoll)
		{
			std::vector<unsigned char> buffer(10000);
			nbytes = recv(objectPoll.getPfd()[i].fd, &buffer[0], sizeof(buffer), MSG_DONTWAIT);	
			if (nbytes <= 0)
			{
				close(objectPoll.getPfd()[i].fd);
				objectPoll.deleteFd(i);
			}
			return (buffer);
		}

		struct sockaddr_in const&	getAddress() const
		{
			return (_address);
		}
		int const& 					getSocketFd() const
		{
			return (_socketFd);
		}
		int getAddrlen()
		{
			return (addrlen);
		}
};

struct  					server
{
	std::string 			name;
	int         			port;
	ft_tsocket				sock;
	size_t					max_size;
    std::vector<std::string>    extensions;
};

struct							client
{
	int							socket;
	int							servIndex;
 	std::vector<unsigned char>	answer;
	std::string					statusCode;
	int							sentBytes;
	int							totalSentBytes;
	int							bytesToSend;
};

int		lookClient(int fd, std::vector<struct client> clients)
{
	for (size_t i = 0; i < clients.size(); i++)
	{
		if (fd == clients[i].socket)
			return (i);
	}
	return (-1);
}

int		fdServer(int fd, std::vector<struct server> servers)
{
	for (size_t i = 0; i < servers.size(); i++)
	{
		if (fd == servers[i].sock.getSocketFd())
			return (i);
	}
	return (-1);
}

bool	portAvailable(std::vector<struct server> servers, int port, int i)
{
	int j = 0;

	while (j < i)
	{
		if (servers[j].port == port)
			return (false);
		j++;
	}
	return (true);
}

int main(void)
{
	std::string filename("test.html");
	std::string f_content;
	f_content = readFileIntoString2(filename);

	std::string	imgfilename("cat.jpeg");
	std::string img_content;
	img_content = readFileIntoString2(imgfilename);
	std::string respone;
	std::string imgrespone;
	std::cout << f_content.size() << std::endl;
	respone = "HTTP/1.1 200 OK\nDate: Thu. 20 May 2004 21:12:58 GMT\nConnection: close\nServer: Apache/1.3.27\nContent-Type: text/html\nContent-Length: 130\n\r\n";
	imgrespone = "HTTP/1.1 200 OK\nDate: Thu. 20 May 2004 21:12:58 GMT\nConnection: close\nServer: Apache/1.3.27\nContent-Type: image/jpeg\n\r\n";

	respone.append(f_content);
	imgrespone.append(img_content);

	std::string termin;
	termin = "\r\n\r\n";

    PollFd poFD;
    std::vector<struct server> servers;
    std::vector<struct client> clients;

    for (int x = 0; x < 3; x++)
    {
        struct server s;
        s.port = (9999 - x);
        servers.push_back(s);
    }

	while(1)
	{

        poll(poFD.getPfd().data(), poFD.getFdCount(), 0); 
        if (poFD.getFdCount() == 0)
        {
            for (size_t j = 0; j < servers.size(); j++)
            {
                servers[j].sock = ft_tsocket(servers[j].port);
                servers[j].sock.setToNonBlocking();
                if (portAvailable(servers, servers[j].port, j))
                {
                    servers[j].sock.socketBind();
                    servers[j].sock.socketListen();
                }
                poFD.addFd(servers[j].sock.getSocketFd());
                //std::cout << poFD.getFdCount() << std::endl;
            }
        }
        for (size_t i(0); i < poFD.getFdCount(); i++)
        {
            if (poFD.getPfd()[i].revents & POLLIN) 
            {
                int k;
                if ((k = fdServer(poFD.getPfd()[i].fd, servers)) != -1)
                {
                    client c;
                    c.socket = servers[k].sock.socketAccept();
                    fcntl(c.socket, F_SETFL, O_NONBLOCK);
                    c.servIndex = k;
                    c.statusCode = "0";
                    poFD.addFd(c.socket);
                    clients.push_back(c);
                }
                else 
                {
                    int k;
                    k = lookClient(poFD.getPfd()[i].fd, clients);
                    std::vector<unsigned char> request;
                    request = servers[clients[k].servIndex].sock.socketRecv(i, poFD);
                    if (request.size() <= 0)
                    {
                        std::vector<struct client>::iterator it(clients.begin());
                        for (int m = 0; m < k; m++)
                            it++;
                        clients.erase(it);
                    }
                    else
                    {
                        //clients[k].answer = reinterpret_cast<unsigned char*>(respone.c_str());
                        //clients[k].answer = reinterpret_cast<unsigned char*>(respone.c_str());
                        // clients[k].answer = reinterpret_cast<unsigned char*>(const_cast<char*>(respone.c_str()));
                        // clients[k].bytesToSend = clients[k].answer.size();
                        clients[k].totalSentBytes = 0;
                    }
                }
            }
            if (i < poFD.getFdCount())
            {
                if (poFD.getPfd()[i].revents & POLLOUT)
                {
                    int k;
                    if ((k = lookClient(poFD.getPfd()[i].fd, clients)) != -1)
                    {
                        //std::cout << clients[k].answer.size() << std::endl;
                        if (clients[k].answer.size() >= 0)
                        {
		std::cout << "1" << std::endl;
                            clients[k].sentBytes = servers[clients[k].servIndex].sock.socketSend(poFD.getPfd()[i].fd, respone.c_str());
                            clients[k].totalSentBytes+= clients[k].sentBytes;
			    //return when answer parsing is done
                            //clients[k].answer.erase(clients[k].answer.begin(), clients[k].answer.begin() + clients[k].sentBytes);
                            if (clients[k].answer.size() == 0)
                            {
								//somehow handle error status codes
                                if (clients[k].statusCode == "413")
                                {
                                    close(poFD.getPfd()[i].fd);
                                    poFD.deleteFd(i);
                                }
                                if (clients[k].statusCode == "301")
                                    sleep(10);
                                clients[k].answer.clear();
                             }
                        }
                    }
                }
            }
        }
	}

    return 0;
}
