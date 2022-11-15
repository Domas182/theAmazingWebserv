
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
#include "../Colors.hpp"
#include "../Request/RequestParser.hpp"

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
		bool						_readyFlag;
		bool						_hasBody;
		bool						_isChunked;
		int							_tmpCnt;
		std::string					_response;
		int							_bytesToSend;
		std::vector<unsigned char> 	_request;
		std::vector<unsigned char>	_body;
		RequestParser				_RP;
		

	public:
		Client();
		Client(int index, int socket);
		~Client();

		RequestParser 		RP;
		void bytesEsize();
		void clearResponse();

		int					&getSock();
		int					getIndex();

		bool				getFlag();
		bool				getBFlag();
		bool				getRFlag();
		bool				getHBFlag();
		bool				getCFlag();
		size_t				getBodySize();
		int					getCnt();
		std::string			getResponse();
		size_t				getResponseSize();
		std::string			getStatusCode();
		std::vector<unsigned char> & getRequest();
		std::vector<unsigned char> & getBody();
		RequestParser &		getRP();


		void		setFlagT();
		void		setFlagF();
    
		void		setBFlagT();
		void		setBFlagF();
    
		void		setRFlagT();
		void		setRFlagF();
    
		void		setHBFlagT();
		void		setHBFlagF();
    
		void		setCFlagT();
		void		setCFlagF();
    
		void		setCnt(int i);
		void		setResp(std::string resp);
		void		setSentBytes(int bytes);
		void		setTotalSentBytes();
		void 		eraseSentBit();
		void		setStatusCode(std::string);
		void 		pushRequest(unsigned char c);
		void		pushBody(unsigned char c);
        void 		printRequest();
        void 		printBody();
		void		clearRequest();

		void		resetClient();

		//temp request holder
		std::vector<unsigned char> tmpReq;
		std::vector<unsigned char> tmpBody;
		std::vector<unsigned char> tmpExtract;
		std::vector<unsigned char> tmpChunkedBody;
		std::string iHex;
		size_t chunkSize;
		int		tmpLen;
		bool	chunkSizeSet;
};


// TODO:integragte the requestParser in teh Client to make it better accesable!
#endif /* CLIENT_HPP */
