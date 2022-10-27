
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
		bool						_bodyFlag;
		int							_tmpCnt;
		std::string					_response;
		int							_bytesToSend;
		std::vector<unsigned char> 	_request;
		std::vector<unsigned char>	_body;
		

	public:
		Client();
		Client(int index, int socket);
		~Client();

		void bytesEsize();
		void clearResponse();

		int					getSock();
		int					getIndex();
		bool				getFlag();
		bool				getBFlag();
		size_t				getBodySize();
		int					getCnt();
		std::string			getResponse();
		size_t				getResponseSize();
		std::string			getStatusCode();
		std::vector<unsigned char> & getRequest();

		void		setFlagT();
		void		setFlagF();
		void		setBFlagT();
		void		setBFlagF();
		void		setCnt(int i);
		void		setResp(std::string resp);
		void		setSentBytes(int bytes);
		void		setTotalSentBytes();
		void		setStatusCode(std::string);
		void 		pushRequest(unsigned char c);
		void		pushBody(unsigned char c);
        void 		printRequest();
        void 		printBody();
};


// TODO:integragte the requestParser in teh Client to make it better accesable!
#endif /* CLIENT_HPP */