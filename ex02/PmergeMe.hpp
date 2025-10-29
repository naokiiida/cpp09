#ifndef PMERGEME_HPP
# define PMERGEME_HPP

#include <vector>
#include <deque>

class PmergeMe
{
    public:
        PmergeMe(void);
        PmergeMe(const PmergeMe& other);
        PmergeMe &operator=(const PmergeMe &other);
        ~PmergeMe();
        PmergeMe(int ac, char **av);
        PmergeMe(std::vector<int> &vec);
        void sort();
        void print();
    private:
        void mergeInsertionSort(std::vector<int>& vec);
        void mergeSort(std::vector<int>& vec);
        void parseArgs(int ac, char **av);
        std::vector<int> _vec;
        std::deque<int> _deque;
};

#endif
