
#include <stack>
#include <iostream>
#include <cstdlib>
#include <exception>
#include <string>
#include "fjNum.hpp"

fjNum::fjNum(fjNum const &src) { this->operator=(src); }
fjNum & fjNum::operator=(fjNum const &rhs)
{    if (this != &rhs)
    {
        this->number = rhs.number;
        this->pending = rhs.pending;
    }
    return *this;
}

fjNum::~fjNum()
{
    //need to clear the stack to prevent memory leaks
    while (!pending.empty())
        pending.pop();
}

const int & fjNum::getNumber() const
{
    return this->number;
}

const std::stack<int>& fjNum::getPending() const
{
    return this->pending;
}

void fjNum::setNumber(int n)
{
    this->number = n;
}
void fjNum::addPending(int n)
{
    this->pending.push(n);
}
