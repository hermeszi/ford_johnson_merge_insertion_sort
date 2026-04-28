#include <iostream>
#include <sstream>
#include <cstdlib>
#include <exception>
#include <algorithm>
#include <string>
#include <ctime>
#include <iomanip>
#include "fjNum.hpp"
#include "PmergeMe.hpp"
#define MILLION 1000000


bool PmergeMe::isVSorted() const
{
    if (sortV.size() != input.size())
        return false;
    for (size_t i = 1; i < sortV.size(); ++i)
    {
        if (sortV[i].getNumber() < sortV[i - 1].getNumber())
            return false;
    }
    return true;
}

struct CountingComparator
{
    size_t& count;
    CountingComparator(size_t& c) : count(c) {}
    bool operator()(const fjNum& a, const fjNum& b) const
    {
        ++count;
        return a < b;
    }
};

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
        
        PmergeMe::comparisonCount = 0; // reset comparison count before sorting

        clock_t startV = clock();
        this->sortVector();
        clock_t endV = clock();
        std::cout << "After  : " << sortV << std::endl;

        if (!isVSorted())
        {
            std::cerr << "Error: Vector is not sorted correctly" << std::endl;
            return 1;
        }
        else
        {
            std::cout << "Vector is sorted successfully" << std::endl;
        }
        double timeV = (static_cast<double>(endV - startV) / CLOCKS_PER_SEC) * MILLION;
        std::cout   << "Time to process a range of " << size << " elements with std::vector : " 
                    << std::fixed << timeV << " us" << std::endl;
        std::cout   << "Number of comparisons made during sorting: " << this->comparisonCount << std::endl;

        // clock_t startD = clock();
        // this->sortDeque(); 
        // clock_t endD = clock();
        // //std::cout << "After (deque) : " << sortD << std::endl;

        //double timeD = (static_cast<double>(endD - startD) / CLOCKS_PER_SEC) * MILLION;

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

std::vector<size_t> PmergeMe::generateOrder(size_t n)
{
    std::vector<size_t> jacobsthal = generateJacobsthal(n);
    //std::cout << "Jacobsthal sequence: " << jacobsthal << std::endl;
    std::vector<size_t> order;
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
        newPairs.push_back(mkPair(receivedPairs.at(i).alpha, receivedPairs.at(i + 1).alpha));
    }

    // 4. recurse: FJSort(new pairs)
    mainChain = FJSort <Container>(newPairs); //mainChain comes back sorted

    /*=========debug message==============*/
    std::cout << "\n--- FJSort level ---\n";
    std::cout << "mainChain after recursion: ";
    for (size_t i = 0; i < mainChain.size(); ++i)
        std::cout << "[" << i << "]=" << mainChain[i].getNumber() << " ";
    std::cout << "\n";


    // 5. get insert order for pend elements, which is based on Jacobsthal sequence.
    //The number of items we recieved from mainChain will be the number of Jocobsthal number we need.
    std::vector<int> localInsertationOrder;
    for (size_t i = 0; i < this->insertionOrder.size(); ++i)
    {
        if (static_cast<size_t>(this->insertionOrder[i]) < receivedPairs.size())
            localInsertationOrder.push_back(insertionOrder[i]);
    }
    // 6. insert remaining pend elements into mainChain using binary search
    // snapshot originals before any insertions -> Jacobsthal indices index into this
    
    //retrive the original fjNums that we need to insert from the pairs, and store them in the same order as mainChain so we can track where they are as we insert
    // std::vector<fjNum> pending(mainChain.size());
    // std::vector<bool> hasPending(mainChain.size(), false);

    // //debug print to check the pending elements before insertion
    // for (size_t i = 0; i < mainChain.size(); ++i)
    // {
    //     std::cout << "mainChain[" << i << "] alpha="
    //             << mainChain[i].getNumber();

    //     if (!mainChain.at(i).getPending().empty())
    //     {
    //         std::cout << " has pending beta="
    //                 << mainChain.at(i).getPending().top().getNumber()
    //                 << "\n";

    //         pending[i] = mainChain.at(i).getPending().top();
    //         hasPending[i] = true;
    //         mainChain.at(i).pop_last_pending();
    //     }
    //     else
    //     {
    //         std::cout << " has NO pending\n";
    //     }
    // }
    // currentIdx[k] tracks where originals[k] currently sits in the growing mainChain
    std::vector<size_t> currentIdx(mainChain.size());
    for (size_t i = 0; i < currentIdx.size(); ++i)
        currentIdx[i] = i;  // before any insertions, original[k] is at index k

    // // == debug print to check if we tracked the pending elements correctly before insertion ==
    // std::cout << "local insertion order: ";
    // for (size_t i = 0; i < localInsertationOrder.size(); ++i)
    //     std::cout << localInsertationOrder[i] << " ";
    // std::cout << "\n";

    // std::cout << "pending table:\n";
    // for (size_t i = 0; i < pending.size(); ++i)
    // {
    //     std::cout << "  [" << i << "] ";
    //     if (hasPending[i])
    //         std::cout << pending[i].getNumber();
    //     else
    //         std::cout << "EMPTY";
    //     std::cout << "\n";
    // }
    while (!localInsertationOrder.empty())
    {
        size_t k = localInsertationOrder.front();         // Jacobsthal-ordered index into originals
        localInsertationOrder.erase(localInsertationOrder.begin());

        // std::cout << "\nTrying k=" << k << "\n";

        // if (k >= pending.size() || !hasPending[k])
        // {
        //     std::cout << "SKIP k=" << k << " no pending\n";
        //     continue;
        // }

        size_t alphaIndex = currentIdx[k];

        if (mainChain[alphaIndex].getPending().empty())
        {
            std::cout << "SKIP: no pending for alpha "
                    << mainChain[alphaIndex].getNumber() << "\n";
            continue;
        }

        fjNum beta = mainChain[alphaIndex].popPending();

        std::cout << "Insert beta=" << beta.getNumber()
                << " before alpha index k=" << k
                << " currentIdx[k]=" << currentIdx[k]
                << " alpha=" << mainChain[currentIdx[k]].getNumber()
                << "\n";

        // originals[k] is the upper bound: beta < originals[k] is already known, no search needed
        typename Container::iterator upperBound = mainChain.begin() + currentIdx[k];
        // binary search only within [begin, upperBound) — comparisons against unknown elements
        typename Container::iterator pos = std::lower_bound(mainChain.begin(), upperBound, beta, CountingComparator(comparisonCount));

        size_t insertedAt = pos - mainChain.begin();
        mainChain.insert(pos, beta);

        std::cout << "Inserted at index " << insertedAt << "\n";
        std::cout << "mainChain now: ";
        for (size_t i = 0; i < mainChain.size(); ++i)
            std::cout << mainChain[i].getNumber() << " ";
        std::cout << "\n";

        // every original at or past the insertion point shifted right by one
        for (size_t i = 0; i < currentIdx.size(); ++i)
            if (currentIdx[i] >= insertedAt)
                currentIdx[i]++;
    }

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
