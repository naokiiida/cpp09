#include "PmergeMe.hpp"
#include <iostream>

// Default constructor
PmergeMe::PmergeMe(void)
{
    std::cout << "Default constructor called\n";
}

// Copy constructor
PmergeMe::PmergeMe(const PmergeMe &other)
{
    std::cout << "Copy constructor called\n";
    (void) other;
}

// Assignment operator overload
PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
    std::cout << "Assignment operator called\n";
    if (this != &other)
    {
        (void) other;
    }
    return (*this);
}

// Destructor
PmergeMe::~PmergeMe(void)
{
    std::cout << "Destructor called\n";
}

PmergeMe::PmergeMe(std::vector<int> &vec) : _vec(vec)
{
    std::cout << "Constructor with vector called\n";
}

PmergeMe::PmergeMe(int ac, char **av)
{
    std::cout << "Constructor with arguments called\n";
    try
    {
        parseArgs(ac, av);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        exit(1);
    }
}

void PmergeMe::parseArgs(int ac, char **av)
{
    std::cout << "Parsing arguments\n";
    for (int i = 1; i < ac; i++)
    {
        std::string arg = av[i];
        if (arg.find_first_not_of("0123456789") != std::string::npos)
        {
            throw std::invalid_argument("Invalid argument '" + arg + "'");
        }
        std::cout << "Parsed argument: " << arg << "\n";
        _vec.push_back(std::stoi(arg));
    }
}

void PmergeMe::mergeSort(std::vector<int> &vec)
{
    std::cout << "Merge sort called\n";
    if (vec.size() <= 1)
    {
        return;
    }
    std::vector<int> left(vec.begin(), vec.begin() + vec.size() / 2);
    std::vector<int> right(vec.begin() + vec.size() / 2, vec.end());
    mergeSort(left);
    mergeSort(right);
    std::merge(left.begin(), left.end(), right.begin(), right.end(), vec.begin());
}

void PmergeMe::mergeInsertionSort(std::vector<int> &vec)
{
    std::cout << "Merge-insert sort called\n";
    if (vec.size() <= 1)
    {
        return;
    }
    std::vector<int> left(vec.begin(), vec.begin() + vec.size() / 2);
    std::vector<int> right(vec.begin() + vec.size() / 2, vec.end());
    mergeSort(left);
    mergeSort(right);
    std::merge(left.begin(), left.end(), right.begin(), right.end(), vec.begin());
}
