#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <iostream>
class PmergeMe
{
public:
    PmergeMe() : input(), sortV(), sortD(){}
    PmergeMe(const std::vector<int>& input) : input(input), sortV(), sortD() {}
    PmergeMe(PmergeMe const &src);
    PmergeMe &operator=(PmergeMe const &rhs);
    ~PmergeMe(){};

    int run ();
    const std::vector<int>& getSortV() const;
    const std::deque<int>& getSortD() const;
private:
    std::vector<int> input;
    std::vector<int> insertOrder;
    std::vector<int> sortV;
    std::deque<int>  sortD;
    
    struct pair
    {
        int alpha;
        int beta;
    };

    template <typename Container>
    Container FJSort(std::vector<PmergeMe::pair> &receivedPairs);

    PmergeMe::pair mkPair(int a, int b);
    std::vector<int> generateOrder(size_t n);
    void sortVector();
    void sortDeque();
};

#endif
