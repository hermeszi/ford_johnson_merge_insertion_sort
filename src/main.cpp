#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <limits>
#include "PmergeMe.hpp"

template <typename Container>
static bool isSorted(const Container& c)
{
    for (size_t i = 1; i < c.size(); ++i)
    {
        if (c[i].getNumber() < c[i - 1].getNumber())
            return false;
    }
    return true;
}

static bool isPositiveInt(const std::string &s)
{
    if (s.empty()) return false;

    for (std::string::size_type i = 0; i < s.length(); ++i)
    {
        if (!std::isdigit(s[i])) return false;
    }

    std::stringstream iss(s);
    long value;
    iss >> value;

    if (iss.fail() || !iss.eof())
        return false;
    if (value <= 0 || value > std::numeric_limits<int>::max())
        return false;

    return true;
}

static bool parseArgs(int argc, char **argv, std::vector<int> &input)
{
    for (int i = 1; i < argc; ++i)
    {
        std::string token(argv[i]);

        if (!isPositiveInt(token))
            return false;

        std::stringstream iss(token);
        int value;
        iss >> value;
        input.push_back(value);
    }
    return true;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Error\n" << "Example:\t";
        std::cerr << "./PmergeMe 3 5 9 7 4" << std::endl;
        return 1;
    }

    std::vector<int> input;
    if (!parseArgs(argc, argv, input))
    {
        std::cerr << "Error" << std::endl;
        return 1;
    }

    try
    {
        PmergeMe sorter(input);
        sorter.run();
        
        if (!isSorted(sorter.getSortV()))
        {
            std::cerr << "Error: Vector container is not sorted" << std::endl;
            return 1;
        }
        // else if (!isSorted(sorter.getSortD()))
        // {
        //     std::cerr << "Error: Deque container is not sorted" << std::endl;
        //     return 1;
        // }
        else
        {
            std::cout << "Vector and deque are sorted successfully" << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
