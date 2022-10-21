#include "Operator.hpp"

#include <cstdlib>
#include <iostream>

int	infile_format(std::string infile)
{
	std::size_t point = infile.find_last_of(".");
	infile = infile.substr(point + 1);
	if (infile.compare("conf") != 0)
		return (1);
	return (0);
}



int main(int argc, char** argv)
{
	if (argc > 2)
	{
		std::cerr << "\nError❗\nPlease enter: "
					<< argv[0] << " <config file>\n" << std::endl;
		return EXIT_FAILURE;
	}
	std::string file;
	if (argc == 2)
	{
		if (!infile_format(argv[1]))
			file = argv[1];
		else
		{
			std::cerr << "\nError❗\n"
				<<"Wrong infile format\n" << std::endl;
			return EXIT_FAILURE;
		}
	}
	else
		file = "config_files/default.conf";
	
	try
	{
		Operator operate(file);
		operate.start_parsing();
		std::cout << operate << std::endl;
		operate.start_process();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}
	// system ("leaks webserv");
	return EXIT_SUCCESS;
}

// void requestReady(std::vector<unsigned char> request, Client& client, size_t bytes)
// {
// 	size_t i = 0;
// 	if (client.getFlag() == false)
// 	{
// //		while (i < bytes && request[i] != '\0' && client.getFlag() != true)

// 		while (i < bytes && request[i] != '\0' && client.getFlag() == false)
// 		{
// 			if (request[i] == '\r' && request[i+1] == '\n' && request[i+2] == '\r' && request[i+3] == '\n')
// 			{
// 				client.printRequest();
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
// 					std::unordered_map<std::string, std::string>::const_iterator got = RP.getRequestH().find("Content-Length");
// 					int contLen = atoi(got->second.c_str());
// 					std::cout << contLen << std::endl;
// 					for (int j = 0; j < contLen; j++)
// 					{
// 						client.pushBody(request[i]);
// 						i++;
// 					}
// 				}
// 			} else {
// 				client.pushRequest(request[i++]);
// 			}
// 		}
// //		if (request[0] == 'P' && request[1] == 'O' && request[2] == 'S' && request[3] == 'T' && client.getFlag() == true)
// //		{
// //			client.setBFlagT();
// //			i += 4;
// //			std::vector<unsigned char>::iterator it = request.begin();
// //			std::advance(it, i);
// //			std::cout << *it << " << this is it || and this is i >> " << i << std::endl;
// //			while (it != request.end())
// //			{
// //				std::cout << request[i];
// //				client.pushBody(request[i]);
// //				it++;
// //				i++;
// //			}
// //			std::cout << "\n\n\n\n\n\n\nWe got post!" << std::endl;
// //		}
// 	}
// }