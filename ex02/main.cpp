#include "PmergeMe.hpp"
#include <iostream>

int main(int ac, char **av)
{
	if (ac < 2)
	{
		std::cerr << "Error: Invalid number of arguments" << std::endl;
		return 1;
	}
	try
	{
		PmergeMe pmerge(ac - 1, av + 1);
		// pmerge.sort();
		// pmerge.print();
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}
