#include "Operator.hpp"
#include "Server.hpp"
#include "Poller.hpp"
#include "Client.hpp"
#include "stdlib.h"
#include "../Request/RequestParser.hpp"
#include "../Request/Handler.hpp"
#include "../Response/Response.hpp"


int fdServer(int fd, std::vector<Server> servers)
{
	for (size_t i = 0; i < servers.size(); i++)
	{
		if (fd == servers[i].getSockFd())
			return (i);
	}
	return (-1);
}

int lookClient(int fd, std::vector<Client> clients)
{
	for (size_t i = 0; i < clients.size(); i++)
	{
		if (fd == clients[i].getSock())
			return (i);
	}
	return (-1);
}

int requestReady(std::vector<unsigned char>& request, Client& client, size_t bytes)
{
		// for (size_t f = 0; f < request.size(); f++)
		// 	std::cout << LB << request[f] << RESET;
		size_t i = 0;
		for (; i < bytes; i++)
		{
			if (request[i] == '\r' && request[i+1] == '\n' && request[i+2] == '\r' && request[i+3] == '\n')
			{
				client.setFlagT();
				i += 4;
				for (int k = 0; k < 2; k++)
				{
					client.pushRequest('\r');
					client.pushRequest('\n');
				}	
				break;
			}
			client.pushRequest(request[i]);
		}
		//client.printRequest();
		return (i);
}

size_t	findBodyLength(Client& client)
{
	if (client.getFlag() == true)	
	{
		RequestParser RP(client.getRequest());
		std::unordered_map<std::string, std::string>::const_iterator got = RP.getRequestH().find("Content-Length");
		size_t contLen = atoi(got->second.c_str());
		std::cout << contLen << std::endl;	
		return (contLen);
	}
	return (0);

}
void extractBody(std::vector<unsigned char>& request, int index, size_t contLen, Client & client)
{
	for (size_t i = 0; i < contLen; i++)
	{
		client.pushBody(request[index + i]);
	}
	if (client.getBodySize() == contLen)
		client.setBFlagT();

}

		// 	// size_t j = 0;
		// 	// for (; j < contLen; j++)
		// 	// {
		// 	// 	client.pushBody(request[i]);
		// 	// 	i++;
		// 	// }

		// 	while (client.getBodySize() != contLen)
		// 	{
		// 	}
		// 	std::cout << "body size: " << client.getBodySize() << "cont len: " << contLen;
		// 	std::cout << client.getBodySize() << ": body" << std::endl;
		// 	if (client.getBodySize() == contLen)
		// 	// if (j == contLen)
		// 	// 	client.setBFlagT();
			
		// }
		
	
		//client.printRequest();
// }
// {
// 	size_t i = 0;
// 	if (client.getFlag() == false)
// 	{
// //		while (i < bytes && request[i] != '\0' && client.getFlag() != true)

// 		while (i < bytes && request[i] != '\0' && client.getFlag() == false)
// 		{
// 			if (request[i] == '\r' && request[i+1] == '\n' && request[i+2] == '\r' && request[i+3] == '\n')
// 			{
// 				// client.printRequest();
// 				client.setFlagT();
// 				i += 4;
// 				for (int k = 0; k < 2; k++)
// 				{
// 					client.pushRequest('\r');
// 					client.pushRequest('\n');
// 				}
// 				// if (request[0] == 'P' && request[1] == 'O' && request[2] == 'S' && request[3] == 'T')
// 				// {
// 					std::cout << "ARE WE JERE??" << std::endl;
// 					RequestParser RP(client.getRequest());
// 					if (RP.getMethod() == "POST")
// 					{
// 						std::unordered_map<std::string, std::string>::const_iterator got = RP.getRequestH().find("Content-Length");
// 						int contLen = atoi(got->second.c_str());
// 						std::cout << contLen << std::endl;
						
// 						// for (int j = 0; j < contLen; j++)
// 						// {
// 						// 	client.pushBody(request[i]);
// 						// 	i++;
// 						// }
// 					}
// 					//RP._body = client._body
// 			} else {
// 				client.pushRequest(request[i++]);
// 			}
// 		}
// 	}
// }

void Operator::start_process()
{
	for (size_t i = 0; i < _servers.size(); ++i)
	{
	// 	if (_servers[i].setF_Content())
	// 		throw std::invalid_argument("Error❗\nCould not open index file");
	// 	if (_servers[i].setImg_Content())
	// 		throw std::invalid_argument("Error❗\nCould not open image file");
	// 	if (_servers[i].setFavi_Content())
	// 		throw std::invalid_argument("Error❗\nCould not open image file");
	
	// 	_servers[i].setResponse();
	// 	_servers[i].setImg_Response();
	// 	_servers[i].setFavi_Response();
		_servers[i].bindPort();
	}
	std::string termin;
	termin = "\r\n\r\n";

	PollFd poFD;

	std::vector<Client> clients;
	//TODO:ASK SONJA 

	while(1)
	{
		poll(poFD.getPfd().data(), poFD.getFdCount(), 0); 
		if (poFD.getFdCount() == 0)
		{
			for (size_t j = 0; j < _servers.size(); j++)
			{
				_servers[j].continueSetup(_servers, j);
				poFD.addFd(_servers[j].getSockFd());
			}
		}
		std::vector<unsigned char> request;
		for (size_t i = 0; i < poFD.getFdCount(); i++)
		{
			if (poFD.getPfd()[i].revents & POLLIN) 
			{
				int k;
				if ((k = fdServer(poFD.getPfd()[i].fd, _servers)) != -1)
				{
					//new
					Client clie(k, _servers[k].sockAccept());
					poFD.addFd(clie.getSock());
					clients.push_back(clie);
				}
				else 
				{
					int k;
					k = lookClient(poFD.getPfd()[i].fd, clients);
					request = _servers[clients[k].getIndex()].sockRecv(i, poFD);
					// 	NOW THERES STUFF IN THE BUFFER TAHT HOLDS THE REQUEST
					if (request.size() <= 0)
					{
						std::vector<Client>::iterator it(clients.begin());
						for (int m = 0; m < k; m++)
							it++;
						clients.erase(it);
					}
					else
					{
						int index = requestReady(request, clients[k], _servers[clients[k].getIndex()].getNBytes());
						if (clients[k].getFlag() == true)
						{
							extractBody(request, index, findBodyLength(clients[k]), clients[k]);
							clients[k].printBody();
							for (size_t f = 0; f < request.size(); f++)
								std::cout << LB << request[f] << RESET;
							extractBody(request, index, findBodyLength(clients[k]), clients[k]);
							clients[k].setFlagF();

						}
						for (size_t f = 0; f < request.size(); f++)
							std::cout << LB << request[f] << RESET;						
						// if (clients[k].getFlag() == true)
						// 	clients[k].printRequest();
						// if (clients[k].getBFlag() == true)
						// 	clients[k].printBody();
						// clients[k].setBFlagF();
						// clients[k].setFlagF();
				
						// if (clients[k].getFlag() == true)
						// {
						// 	RequestParser RP(clients[k].getRequest());
						// 	clients[k].printBody();	
						// 	// Handler H(RP);
						// 	// H.start_handling(_servers[clients[k].getIndex()]);

						// 	// if (_servers[clients[k].getIndex()].setF_Content())
						// 	// 	throw std::invalid_argument("Error❗\nCould not open index file");
						// 	// if (_servers[clients[k].getIndex()].setImg_Content())
						// 	// 	throw std::invalid_argument("Error❗\nCould not open image file");
						// 	// if (_servers[clients[k].getIndex()].setFavi_Content())
						// 	// 	throw std::invalid_argument("Error❗\nCould not open image file");
						// 	// _servers[clients[k].getIndex()].setResponse();
						// 	// _servers[clients[k].getIndex()].setImg_Response();
						// 	// _servers[clients[k].getIndex()].setFavi_Response();


						// 	if (RP.getMethod() == "GET")
						// 	{
						// 		if (RP.getURI() == "/favicon.ico")
						// 			clients[k].setResp(_servers[clients[k].getIndex()].getFavi_Response());
						// 		else if (RP.getURI() == "/cat.jpeg")				
						// 			clients[k].setResp(_servers[clients[k].getIndex()].getImg_Response());
						// 		else if (RP.getURI() == "/")
						// 			clients[k].setResp(_servers[clients[k].getIndex()].getResponse());
						// 		clients[k].getRequest().clear();
						// 		clients[k].setFlagF();
						// 	} else {
						// 		clients[k].setResp("");
						// 		clients[k].setFlagF();
						// 	}
						// }
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
						if (clients[k].getResponseSize() >= 0)
						{
							//new
							

							_servers[clients[k].getIndex()].sockSend(poFD.getPfd()[i].fd, clients[k]);
							//std::cout << clients[k].getResponseSize() << std::endl;
							clients[k].clearResponse();
							//if (clients[k].getResponseSize() == 0)
							//{
							//	//somehow handle error status codes
							//	if (clients[k].getStatusCode() == "420")
							//	{
							//		close(poFD.getPfd()[i].fd);
							//		poFD.deleteFd(i);
							//	}
							//	if (clients[k].getStatusCode() == "69")
							//		sleep(10);
							//	clients[k].clearResponse();
							//}
						}
					}
				}
			}
		}
	}
}
