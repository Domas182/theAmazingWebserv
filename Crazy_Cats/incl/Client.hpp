
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "CrazyCats.hpp"
#include "RequestParser.hpp"

// ************************************************************************** //
//                               Client Class                                 //
// ************************************************************************** //

class Client
{
	private:
		int								_socket;
		int								_sIndex;
		std::vector<unsigned char>		_answer;
		std::string						_statCode;
		int								_sentBytes;
		int								_totalSentBytes;
		bool							_requestFlag;
		bool							_bodyFlag;
		bool							_readyFlag;
		bool							_hasBody;
		bool							_isChunked;
		bool							_headerTooBig;
		int								_tmpCnt;
		std::string						_response;
		int								_bytesToSend;
		std::vector<unsigned char> 		_request;
		std::vector<unsigned char>		_body;
		RequestParser					_RP;
		

	public:
		std::vector<unsigned char> 		tmpReq;
		std::vector<unsigned char> 		tmpBody;
		std::vector<unsigned char> 		tmpExtract;
		std::vector<unsigned char> 		tmpChunkedBody;
		std::string 					iHex;
		size_t 							chunkSize;
		int								tmpLen;
		bool							chunkSizeSet;
		bool							isEmpty;

		Client();
		Client(int index, int socket);
		~Client();
		void 							bytesEsize();
		void 							clearResponse();
		int								&getSock();
		int								getIndex();
		bool							getFlag();
		bool							getBFlag();
		bool							getRFlag();
		bool							getHBFlag();
		bool							getCFlag();
		bool							getH2BFlag();
		size_t							getBodySize();
		int								getCnt();
		std::string						getResponse();
		size_t							getResponseSize();
		std::string						getStatusCode();
		std::vector<unsigned char> & 	getRequest();
		std::vector<unsigned char> & 	getBody();
		RequestParser &					getRP();


		void							setFlagT();
		void							setFlagF();
    
		void							setBFlagT();
		void							setBFlagF();
    
		void							setRFlagT();
		void							setRFlagF();
    
		void							setHBFlagT();
		void							setHBFlagF();
    
		void							setCFlagT();
		void							setCFlagF();
    
		void		setH2BFlagT();
		void		setH2BFlagF();

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

		void		crlfPush();
		void		chunkedHandler(std::vector<unsigned char>& request, size_t& i, size_t bytes);
		void		headerFlagSetter(int len);
		int			findBodyLength(std::vector<unsigned char>& request);
		void		headerCountAndFlags(int len);

		void		resetClient();

		//temp request holder
		// std::vector<unsigned char> tmpReq;
		// std::vector<unsigned char> tmpBody;
		// std::vector<unsigned char> tmpExtract;
		// std::vector<unsigned char> tmpChunkedBody;
		// std::string iHex;
		// size_t chunkSize;
		// int		tmpLen;
		// bool	chunkSizeSet;
};

bool 									crlfBool(std::vector<unsigned char>& data, size_t i);
#endif /* CLIENT_HPP */
