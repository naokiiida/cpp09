#include "BitcoinExchange.hpp"
#include <iostream>

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <input_file>" << "\n";
		return 1;
	}
	BitcoinExchange exchange("data.csv");

	try
	{
		const std::string fileContent = BitcoinExchange::readFile(std::string(argv[1]));
		BitcoinExchange::outputPrices(fileContent);
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error: " << e.what() << "\n";
		return 1;
	}
	return 0;
}
