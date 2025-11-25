#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <utility>
#include <ctime>
#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <limits>
#include <cctype>

class PmergeMe {
public:
    // Orthodox Canonical Form
    PmergeMe(int argc, char **argv);
    PmergeMe(const PmergeMe& other);
    PmergeMe& operator=(const PmergeMe& other);
    ~PmergeMe();

    void run();

private:
    // Private default constructor to prevent instantiation without arguments
    PmergeMe();

    // Containers and timing variables
    std::vector<int> _vec;
    std::deque<int>  _deq;
    std::vector<int> _input;
    double _time_vec;
    double _time_deq;

    // Comparison counter
    mutable size_t _comparisons;

    // Private helper methods
    void parseInput(int argc, char **argv);
    void sortAndMeasure();

    // Ford-Johnson sort implementation for std::vector
    void mergeInsertSort(std::vector<int>& c);

    // Ford-Johnson sort implementation for std::deque
    void mergeInsertSort(std::deque<int>& c);

    // Comparison counting helpers
    size_t getTheoreticalComparisons(size_t n) const;
    size_t countLowerBoundComparisons(size_t distance) const;
    void resetComparisons();
    size_t getComparisons() const;

    // Helper to print a sequence
    template <typename Container>
    void printSequence(const std::string& prefix, const Container& c) {
        std::cout << prefix;
        // Limit printing for large sequences as per subject examples
        const int print_limit = 10;
        int count = 0;
        for (typename Container::const_iterator it = c.begin(); it != c.end() && count < print_limit; ++it) {
            std::cout << *it << " ";
            count++;
        }
        if (c.size() > print_limit) {
            std::cout << "[...]";
        }
        std::cout << std::endl;
    }
};

#endif
