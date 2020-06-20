#pragma once
#include <limits.h>
#include <ostream>
#include <stdexcept>

class udouble
{

private:
    //members
    double d = 0;
public:
    //constructors
    udouble() = default;
    udouble(double x)
    {
        if(x<0) throw std::invalid_argument("x must be positive");
        d = x;
    }

    //functions
    friend std::ostream& operator<<(std::ostream& os, const udouble& ud) { return std::cout<<ud.d; }
    explicit operator double() const { return d; }
    
    udouble operator++() { return (d==__DBL_MAX__)?throw std::out_of_range("unsigned double overflowed"):d+=1; }
    udouble operator--() { return (d>0)?d-=1:throw std::out_of_range("unsigned double can not be negative"); }

    udouble operator+(const udouble& ud) { return udouble(this->d+ud.d); }
    udouble operator-(const udouble& ud) { return udouble(this->d-ud.d); }
    udouble operator*(const udouble& ud) { return udouble(this->d*ud.d); }
    udouble operator/(const udouble& ud) { return udouble(this->d/ud.d); }
    udouble operator%(const udouble& ud) { return udouble( (this->d/ud.d)-((int)(this->d/ud.d) ) ); }

    bool operator<(const udouble& ud) { return this->d<ud.d; }
    bool operator<=(const udouble& ud) { return this->d<=ud.d; }
    bool operator>(const udouble& ud) { return this->d>ud.d; }
    bool operator>=(const udouble& ud) { return this->d>=ud.d; }
    
    bool operator==(const udouble& ud) { return this->d==ud.d; }
    bool operator!=(const udouble& ud) { return this->d!=ud.d; }
};
