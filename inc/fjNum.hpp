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

    bool operator < (const fjNum& other) const
    {
        return this->number < other.number;
    }

    bool operator > (const fjNum& other) const
    {
        return this->number > other.number;
    }

    bool operator == (const fjNum& other) const
    {
        return this->number == other.number;
    }   

    friend std::ostream& operator<<(std::ostream& os, const fjNum& num)
    {
        os << num.getNumber();
        return os;
    }

private:    
    int             number;
    std::stack<int> pending;
};

#endif
