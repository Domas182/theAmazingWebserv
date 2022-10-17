#include "Config.hpp"

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
		Config parser(file);
		parser.start_parsing();
		std::cout << parser << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}
	// system ("leaks webserv");
	return EXIT_SUCCESS;
}
