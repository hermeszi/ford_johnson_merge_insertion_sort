#ifndef FJNUM_HPP
#define FJNUM_HPP

#include <stack>
#include <iostream>
class fjNum
{
public:
    fjNum() : number(), pending() {}
    fjNum(const int & n) : number(n), pending () {}
    fjNum(fjNum const &src);
    fjNum &operator=(fjNum const &rhs);
    ~fjNum();

    const int & getNumber() const;
    const std::stack<int>& getPending() const;

    void setNumber(int n);
    void addPending(int n);

private:    
    int             number;
    std::stack<int> pending;
};

#endif
