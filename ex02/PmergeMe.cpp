#include "PmergeMe.hpp"
#include <cmath>

PmergeMe::PmergeMe(void) : _time_vec(0.0), _time_deq(0.0), _comparisons(0) {}

PmergeMe::PmergeMe(int argc, char **argv) : _time_vec(0.0), _time_deq(0.0), _comparisons(0) {
    parseInput(argc, argv);
}

PmergeMe::PmergeMe(const PmergeMe& other) {
    *this = other;
}

PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
    if (this != &other) {
        _vec = other._vec;
        _deq = other._deq;
        _input = other._input;
        _time_vec = other._time_vec;
        _time_deq = other._time_deq;
    }
    return *this;
}

PmergeMe::~PmergeMe(void) {}

// --- Theoretical Minimum Calculation ---
size_t PmergeMe::getTheoreticalComparisons(size_t n) const {
    if (n <= 1) {
        return 0;
    }
    double log2_factorial = 0.0;
    for (size_t i = 2; i <= n; ++i) {
        log2_factorial += std::log2(static_cast<double>(i));
    }
    return static_cast<size_t>(std::ceil(log2_factorial));
}

// --- Comparison Counting Helpers ---
void PmergeMe::resetComparisons() {
    _comparisons = 0;
}

size_t PmergeMe::getComparisons() const {
    return _comparisons;
}

// Calculate number of comparisons made by std::lower_bound
// Binary search performs ceil(log2(n+1)) comparisons in worst case
size_t PmergeMe::countLowerBoundComparisons(size_t distance) const {
    if (distance == 0) return 0;
    size_t count = 0;
    size_t n = distance;
    while (n > 0) {
        n >>= 1;
        count++;
    }
    return count;
}

// --- Input Parsing ---
void PmergeMe::parseInput(int argc, char **argv) {
    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        std::stringstream ss(arg);
        std::string num_str;

        while (ss >> num_str) {
            for (size_t j = 0; j < num_str.length(); ++j) {
                if (!std::isdigit(num_str[j])) {
                    throw std::invalid_argument("Error: non-digit character in input.");
                }
            }
            long val;
            std::stringstream convert(num_str);
            convert >> val;
            if (val < 0 || val > std::numeric_limits<int>::max()) {
                throw std::invalid_argument("Error: number is negative or out of integer range.");
            }
            _input.push_back(static_cast<int>(val));
        }
    }
    if (_input.empty()) {
        throw std::invalid_argument("Error: no input provided.");
    }
}

// --- Main Execution Logic ---
void PmergeMe::run() {
    printSequence("Before: ", _input);
    sortAndMeasure();
    printSequence("After:  ", _vec);

    std::cout << "Time to process a range of " << _input.size()
              << " elements with std::vector : " << _time_vec << " us\n";
    std::cout << "Time to process a range of " << _input.size()
              << " elements with std::deque  : " << _time_deq << " us\n";
    std::cout << "Number of comparisons made: " << getComparisons() << "\n";
    size_t theoretical_min = getTheoreticalComparisons(_input.size());
    std::cout << "Theoretical minimum comparisons for " << _input.size() << " elements: " << theoretical_min << "\n";
}

void PmergeMe::sortAndMeasure() {
    // Vector (with comparison counting)
    _vec = _input;
    resetComparisons();
    clock_t start_vec = clock();
    mergeInsertSort(_vec);
    clock_t end_vec = clock();
    _time_vec = static_cast<double>(end_vec - start_vec) * 1000000.0 / CLOCKS_PER_SEC;

    // Deque (without comparison counting to avoid duplication)
    _deq.assign(_input.begin(), _input.end());
    clock_t start_deq = clock();
    mergeInsertSort(_deq);
    clock_t end_deq = clock();
    _time_deq = static_cast<double>(end_deq - start_deq) * 1000000.0 / CLOCKS_PER_SEC;
}

// --- Vector Implementation ---
void PmergeMe::mergeInsertSort(std::vector<int>& c) {
    const size_t n = c.size();
    if (n <= 1) return;

    int straggler = -1;
    bool has_straggler = (n % 2 != 0);
    if (has_straggler) {
        straggler = c.back();
        c.pop_back();
    }

    std::vector<std::pair<int, int> > pairs;
    for (size_t i = 0; i < c.size(); i += 2) {
        _comparisons++; // Count pairwise comparison
        if (c[i] > c[i+1]) pairs.push_back(std::make_pair(c[i], c[i+1]));
        else pairs.push_back(std::make_pair(c[i+1], c[i]));
    }

    std::vector<int> main_chain;
    main_chain.reserve(pairs.size());
    for (size_t i = 0; i < pairs.size(); ++i) main_chain.push_back(pairs[i].first);

    mergeInsertSort(main_chain);

    std::vector<std::pair<int, int> > sorted_pairs;
    sorted_pairs.reserve(main_chain.size());
    for (size_t i = 0; i < main_chain.size(); ++i) {
        int b = main_chain[i];
        for (size_t j = 0; j < pairs.size(); ++j) {
            if (pairs[j].first == b) {
                sorted_pairs.push_back(pairs[j]);
                break;
            }
        }
    }

    // Add virtual pair for straggler if present
    if (has_straggler) {
        sorted_pairs.push_back(std::make_pair(std::numeric_limits<int>::max(), straggler));
    }

    c.clear();
    c.reserve(n);
    if (!sorted_pairs.empty()) c.push_back(sorted_pairs[0].second);
    for (size_t i = 0; i < main_chain.size(); ++i) c.push_back(main_chain[i]);

    std::vector<size_t> insertion_order;
    size_t pend_count = sorted_pairs.size();
    size_t last_jacob = 1;
    size_t jacob_idx = 3;
    while (last_jacob < pend_count) {
        size_t end = (jacob_idx < pend_count) ? jacob_idx : pend_count;
        for (size_t i = end; i > last_jacob; --i) insertion_order.push_back(i - 1);
        size_t temp = jacob_idx;
        jacob_idx = jacob_idx + 2 * last_jacob;
        last_jacob = temp;
    }

    for (size_t i = 0; i < insertion_order.size(); ++i) {
        size_t k = insertion_order[i];
        int elem_to_insert = sorted_pairs[k].second;
        int partner_b = sorted_pairs[k].first;
        std::vector<int>::iterator search_end = std::lower_bound(c.begin(), c.end(), partner_b);
        size_t search_range = std::distance(c.begin(), search_end);
        _comparisons += countLowerBoundComparisons(search_range);
        std::vector<int>::iterator insert_pos = std::lower_bound(c.begin(), search_end, elem_to_insert);
        c.insert(insert_pos, elem_to_insert);
    }
}

// --- Deque Implementation ---
void PmergeMe::mergeInsertSort(std::deque<int>& c) {
    const size_t n = c.size();
    if (n <= 1) return;

    int straggler = -1;
    bool has_straggler = (n % 2 != 0);
    if (has_straggler) {
        straggler = c.back();
        c.pop_back();
    }

    std::deque<std::pair<int, int> > pairs;
    for (size_t i = 0; i < c.size(); i += 2) {
        if (c[i] > c[i+1]) pairs.push_back(std::make_pair(c[i], c[i+1]));
        else pairs.push_back(std::make_pair(c[i+1], c[i]));
    }

    std::deque<int> main_chain;
    for (size_t i = 0; i < pairs.size(); ++i) main_chain.push_back(pairs[i].first);

    mergeInsertSort(main_chain);

    std::deque<std::pair<int, int> > sorted_pairs;
    for (size_t i = 0; i < main_chain.size(); ++i) {
        int b = main_chain[i];
        for (size_t j = 0; j < pairs.size(); ++j) {
            if (pairs[j].first == b) {
                sorted_pairs.push_back(pairs[j]);
                break;
            }
        }
    }

    // Add virtual pair for straggler if present
    if (has_straggler) {
        sorted_pairs.push_back(std::make_pair(std::numeric_limits<int>::max(), straggler));
    }

    c.clear();
    if (!sorted_pairs.empty()) c.push_back(sorted_pairs[0].second);
    for (size_t i = 0; i < main_chain.size(); ++i) c.push_back(main_chain[i]);

    std::vector<size_t> insertion_order;
    size_t pend_count = sorted_pairs.size();
    size_t last_jacob = 1;
    size_t jacob_idx = 3;
    while (last_jacob < pend_count) {
        size_t end = (jacob_idx < pend_count) ? jacob_idx : pend_count;
        for (size_t i = end; i > last_jacob; --i) insertion_order.push_back(i - 1);
        size_t temp = jacob_idx;
        jacob_idx = jacob_idx + 2 * last_jacob;
        last_jacob = temp;
    }

    for (size_t i = 0; i < insertion_order.size(); ++i) {
        size_t k = insertion_order[i];
        int elem_to_insert = sorted_pairs[k].second;
        int partner_b = sorted_pairs[k].first;
        std::deque<int>::iterator search_end = std::lower_bound(c.begin(), c.end(), partner_b);
        std::deque<int>::iterator insert_pos = std::lower_bound(c.begin(), search_end, elem_to_insert);
        c.insert(insert_pos, elem_to_insert);
    }
}
