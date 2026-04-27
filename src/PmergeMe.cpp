#include <iostream>
#include <sstream>
#include <cstdlib>
#include <exception>
#include <string>
#include <ctime>
#include <iomanip>
#include "fjNum.hpp"
#include "PmergeMe.hpp"
#define MILLION 1000000

const std::vector<fjNum>& PmergeMe::getSortV() const { return sortV; }
//const std::deque<int>& PmergeMe::getSortD() const { return sortD; }

PmergeMe::PmergeMe() : input(), sortV()//, sortD()
{
    errorFlag = true;
}

PmergeMe::PmergeMe(const std::vector<int>& input) : input(input), sortV()//, sortD() 
{
    errorFlag = false;
}

PmergeMe::PmergeMe(PmergeMe const &src){ this->operator=(src); }
PmergeMe & PmergeMe::operator=(PmergeMe const &rhs)
{
    if (this != &rhs)
    {
        this->sortV = rhs.sortV;
        //this->sortD = rhs.sortD;
        this->input = rhs.input;
        this->insertionOrder = rhs.insertionOrder;
        this->errorFlag = rhs.errorFlag;
    }
    return *this;
}

template <typename T> //printing helper function for vectors, used for debugging
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
    for (size_t i = 0; i < v.size(); ++i)
    {
        os << v[i] << (i != v.size() - 1 ? ", " : "");
    }
    return os;
}
template <typename T> //printing helper function for deques, used for debugging
std::ostream& operator<<(std::ostream& os, const std::deque<T>& d)
{
    for (size_t i = 0; i < d.size(); ++i)
    {                                           
        os << d[i] << (i != d.size() - 1 ? ", " : "");
    }
    return os;
}

template <typename T> //printing helper function for stack, used for debugging
std::ostream& operator<<(std::ostream& os, const std::stack<T>& d)
{
    for (size_t i = 0; i < d.size(); ++i)
    {                                           
        os << d[i] << (i != d.size() - 1 ? ", " : "");
    }
    return os;
}

template <typename Container> //template to convert input vector of ints to vector or deque of fjNums, used for sorting
Container PmergeMe::convertToChain()
{
    Container chain;
    for (size_t i = 0; i < input.size(); ++i)
    {
        chain.push_back(fjNum(input.at(i)));
    }
    return chain;
}

int PmergeMe::run ()
{
    try
    {
        size_t size = input.size();
        if (size == 0)
        {
            std::cerr << "Error: No input provided" << std::endl;
            return 1;
        }

        this->insertionOrder = this->generateOrder(size);
        std::cout << "Insertion order: " << this->insertionOrder << std::endl;

        std::cout << "Before (Raw input): " << input << std::endl;

        sortV = convertToChain<std::vector<fjNum> >();
        std::cout << "Before (vector of fjNums): ";
        for (size_t i = 0; i < sortV.size(); ++i)
        {
            std::cout << sortV.at(i).getNumber() << " ";
        }
        std::cout << std::endl;
        
        clock_t startV = clock();
        this->sortVector();
        clock_t endV = clock();
        std::cout << "After  : " << sortV << std::endl;

        // clock_t startD = clock();
        // this->sortDeque(); 
        // clock_t endD = clock();
        // //std::cout << "After (deque) : " << sortD << std::endl;

        //double timeV = (static_cast<double>(endV - startV) / CLOCKS_PER_SEC) * MILLION;
        //double timeD = (static_cast<double>(endD - startD) / CLOCKS_PER_SEC) * MILLION;

        //std::cout  << "Time to process a range of " << size << " elements with std::vector : " 
         //          << std::fixed << timeV << " us" << std::endl;
        // std::cout  << "Time to process a range of " << size << " elements with std::deque  : " 
        //            << std::fixed << timeD << " us" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
     }

    return 0;
}

PmergeMe::pair PmergeMe::mkPair(fjNum a, fjNum b)
{
    PmergeMe::pair newPair;

    if (a.getNumber() > b.getNumber())
    {
        newPair.alpha = a;
        newPair.beta = b;

        newPair.alpha.addPending(b);
    }
    else
    {
        newPair.alpha = b;
        newPair.beta = a;

        newPair.alpha.addPending(a);
    }
    return newPair;
}

static std::vector<size_t> generateJacobsthal(size_t n)
{
    std::vector<size_t> chain;
    chain.push_back(0);
    chain.push_back(1);
    
    while (chain.back() < n)
    {
        //J(n) = J(n-1) + 2 * J(n-2)
        chain.push_back(chain[chain.size() - 1] + (2 * chain[chain.size() - 2]));
    }
    return chain;
}

std::vector<int> PmergeMe::generateOrder(size_t n)
{
    std::vector<size_t> jacobsthal = generateJacobsthal(n);
    //std::cout << "Jacobsthal sequence: " << jacobsthal << std::endl;
    std::vector<int> order;
    order.push_back(0);
    if (n > 1) order.push_back(1);
    for (size_t i = 3; i < jacobsthal.size(); ++i)
    {
        size_t high = std::min(jacobsthal[i], n - 1);   // if jacobsthal number exceeds n, we only need to go up to n-1
        size_t low = jacobsthal[i - 1] + 1;             // low starts at the index after the previous jacobsthal number, since we want to insert in between the previous and current jacobsthal numbers
        if (low > n - 1) break;                         // if low exceeds n, we are done
        for (size_t j = high; j >= low; --j) 
        {
            order.push_back(j); 
            if (j == 0) break;                          // to prevent underflow
        }
        if (jacobsthal[i] >= n) break;                  // if jacobsthal number exceeds n, we are done
    }
    return order;
}

template <typename Container>
Container PmergeMe::FJSort(std::vector<PmergeMe::pair> &receivedPairs)
{
    Container mainChain;

    // 1. base case: one pair
    if (receivedPairs.size() == 1)
    {   
        mainChain.push_back(receivedPairs.at(0).beta);
        mainChain.push_back(receivedPairs.at(0).alpha);
        return mainChain;
    }

    // 2. check if odd number of pairs -> save last pair as remaining to insert later
    PmergeMe::pair laggard;
    bool hasLaggard = false;
    if (receivedPairs.size() % 2 != 0)
    {   
        hasLaggard = true;
        laggard = receivedPairs.back();
        receivedPairs.pop_back();
    }

    // 3. form new pairs from alphas of current pairs
    std::vector<PmergeMe::pair> newPairs;
    for (size_t i = 0; i < receivedPairs.size(); i+=2)
    {        
        newPairs.push_back(mkPair(receivedPairs.at(i), receivedPairs.at(i + 1)));
    }

    // 4. recurse: FJSort(new pairs)
    mainChain = FJSort <Container>(newPairs);

    // 5. get insert order for pend elements, which is based on Jacobsthal sequence.
    //The number of items we recieved from mainChain will be the number of Jocobsthal number we need.
    std::vector<int> localInsertationOrder;
    for (size_t i = 0; i < insertionInsertationOrder.size(); ++i)
    {
        if (static_cast<size_t>(insertionOrder[i]) < mainChain.size())
            localInsertationOrder.push_back(insertionOrder[i]);
    }
    // 6. insert remaining pend elements into mainChain using binary search
    tempMainChain = mainChain; //so we don't mess with the order of mainChain while inserting
    //we need a algo/data structure to keep track of each new insertation id,
    //and make a function ignore those number when we count the jacobsothal sequence, since the jacobsthal sequence is based on the original mainChain size, but as we insert new elements, the size of mainChain changes, so we need to ignore the new elements when we count the jacobsthal sequence. We can use a set to keep track of the new insertation id, and make a function ignore those number when we count the jacobsthal sequence in the mainchain.
    while (!localInsertationOrder.empty())
    {   
        size_t currentIndex = localInsertationOrder.front();
        localInsertationOrder.erase(localInsertationOrder.begin());
        PmergeMe::pair currentPair = mainChain.at(currentIndex);
        
        typename Container::iterator pos = std::lower_bound(mainChain.begin(), upperBound, currentPair.beta);
        mainChain.insert(pos, currentPair.beta);
    }
    // while (!receivedPairs.empty())
    // {   
    //     PmergeMe::pair currentPair = receivedPairs.front();
    //     receivedPairs.erase(receivedPairs.begin());
    //     typename Container::iterator upperBound = std::lower_bound(mainChain.begin(), mainChain.end(), currentPair.alpha);
    //     typename Container::iterator pos = std::lower_bound(mainChain.begin(), upperBound, currentPair.beta);
    //     mainChain.insert(pos, currentPair.beta);
    // }

    // 6. if remaining exists (from the starting pairing), insert it into mainChain using binary search
    if (hasLaggard)
    {   
        typename Container::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), laggard.alpha);
        mainChain.insert(pos, laggard.alpha);
        typename Container::iterator upperBound = std::lower_bound(mainChain.begin(), mainChain.end(), laggard.alpha);
        pos = std::lower_bound(mainChain.begin(), upperBound, laggard.beta);
        mainChain.insert(pos, laggard.beta);
    }

    return mainChain;
}

void PmergeMe::sortVector()
{
    if (input.size() < 2)
    {
        sortV.at(0).setNumber(input.at(0));
        return ;
    }

    bool        hasLaggard = false;
    fjNum       laggard_value(0);
    
    if (input.size() % 2 != 0)
    {
        hasLaggard = true;
        laggard_value.setNumber(input.back());
        input.pop_back();
    }
    
    std::vector <PmergeMe::pair> vPairChain;
    for (size_t i = 0; i < input.size(); i+=2)
    {
         vPairChain.push_back(mkPair(fjNum(input.at(i)), fjNum(input.at(i + 1))));
    }

    sortV = FJSort<std::vector<fjNum> >(vPairChain);
    if (hasLaggard and !sortV.empty()) // insert laggard into sortV using binary search
    {   
         typename std::vector<fjNum>::iterator pos = std::lower_bound(sortV.begin(), sortV.end(), laggard_value);
         sortV.insert(pos, laggard_value);
    }
}

void PmergeMe::sortDeque()
{
    // if (input.size() < 2)
    // {
    //     sortD.assign(input.begin(), input.end());
    //     return ;
    // }

    // bool        hasLaggard = false;
    // int         laggard = 0;
    
    // if (input.size() % 2 != 0)
    // {
    //     hasLaggard = true;
    //     laggard = input.back();
    //     input.pop_back();
    // }
    
    // std::vector <PmergeMe::pair> vChain;
    // for (size_t i = 0; i < input.size(); i+=2)
    // {
    //     vChain.push_back(mkPair(input.at(i), input.at(i + 1)));
    // }

    // sortD = FJSort<std::deque<int> >(vChain);
    // if (hasLaggard and !sortD.empty())
    // {
    //     // insert laggard into sortD using binary search
    //     std::deque<int>::iterator pos = std::lower_bound(sortD.begin(), sortD.end(), laggard);
    //     sortD.insert(pos, laggard);
    // }   
}
