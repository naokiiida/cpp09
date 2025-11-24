#include "PmergeMe.hpp"
#include <iostream>

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		std::cerr << "Error: Please provide a sequence of positive integers.\n";
		return 1;
	}

	try
	{
		PmergeMe pmergeme(argc, argv);
		pmergeme.run();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}
