
#ifndef CLIENT_HPP
#define CLIENT_HPP

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

// ************************************************************************** //
//                               Client Class                                 //
// ************************************************************************** //

class Client
{
	private:
		int							_socket;
		int							_sIndex;
		std::vector<unsigned char>	_answer;
		std::string					_statCode;
		int							_sentBytes;
		int							_totalSentBytes;
		bool						_requestFlag;
		int							_tmpCnt;
		std::string					_response;
		int							_bytesToSend;

	public:
		Client();
		Client(int index, int socket);
		~Client();

		void bytesEsize();
		void clearResponse();

		int					getSock();
		int					getIndex();
		bool				getFlag();
		int					getCnt();
		std::string			getResponse();
		size_t				getResponseSize();
		std::string			getStatusCode();

		void		setFlagT();
		void		setFlagF();
		void		setCnt(int i);
		void		setResp(std::string resp);
		void		setSentBytes(int bytes);
		void		setTotalSentBytes();
		void		setStatusCode(std::string);
};

#endif /* CLIENT_HPP */