#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <iostream>
#include "fjNum.hpp"
class PmergeMe
{
public:
    PmergeMe();
    PmergeMe(const std::vector<int>& input);
    PmergeMe(PmergeMe const &src);
    PmergeMe &operator=(PmergeMe const &rhs);
    ~PmergeMe(){};
    
    int run ();
    const std::vector<fjNum>& getSortV() const;
    //const std::deque<int>& getSortD() const;
private:
    std::vector<int> input;
    std::vector<size_t> insertionOrder;
    std::vector<fjNum> sortV;
    //std::deque<fjNum>  sortD;

    bool errorFlag;
    struct pair
    {
        fjNum alpha;
        fjNum beta;
    };

    template <typename Container> Container FJSort(std::vector<PmergeMe::pair> &receivedPairs);
    template <typename Container> Container convertToChain(); //convert int vector to fjNum chain
    PmergeMe::pair mkPair(fjNum a, fjNum b);
    std::vector<size_t> generateOrder(size_t n);
    void sortVector();
    void sortDeque();
};

#endif
