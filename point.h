#pragma once
#include <ostream>
#include "generics.h"

template <size_t sz = 0, Number Num = float>
requires ArithmeticOperators<Num> &&EqualityComparable<Num> class Point
{
private:
    Num *members = new Num[sz == 0 ? 1 : sz]{0};

public:
    Point() = default;
    Point(std::initializer_list<Num> list)
    {
        if (list.size() == 0)
            throw std::invalid_argument("list must be atleast 1 item");
        Num *params = new Num[list.size()];
        int j = 0;
        for (Num n : list)
        {
            params[j] = n;
            ++j;
        }

        if (sz < list.size())
            throw std::invalid_argument("list of arguments can't be larger then initilized size");
        for (unsigned i = 0; i < list.size(); ++i)
        {
            this->members[i] = params[i];
        }
        delete[] params;
    }
    Point(const Point<sz, Num> &p) noexcept
    {
        for (unsigned i = 0; i < p.Size(); ++i)
        {
            members[i] = p[i];
        }
    }
    Point<sz, Num> operator=(const Point<sz, Num> &p) noexcept
    {
        for (unsigned i = 0; i < p.sz; ++i)
        {
            members[i] = p[i];
        }
        return *this;
    }
    Point(Point<sz, Num> &&p) noexcept
    {
        members = p.members;
        p.members = nullptr;
    }
    Point<sz, Num> operator=(Point<sz, Num> &&p) noexcept
    {
        members = p.members;
        p.members = nullptr;
        return *this;
    }
    ~Point() { delete[] members; }

    size_t Size() const
    {
        return sz;
    }
    friend std::ostream &operator<<(std::ostream &os, const Point<sz, Num> &p)
    {
        os << "{ ";
        for (int i = 0; i < sz; ++i)
        {
            os << p[i] << ((i == sz - 1) ? " }" : ", ");
        }
        return os;
    }
    Num &operator[](int pos)
    {
        if (pos >= sz || pos < 0)
            throw new std::out_of_range("can't index out of size");
        return members[pos];
    }
    Num operator[](int pos) const
    {
        if (pos >= sz || pos < 0)
            throw new std::out_of_range("can't index out of size");
        return members[pos];
    }

    //arithmetic operators
    Point<sz, Num> operator+(const Point<sz, Num> &a) const
    {
        Point<sz, Num> res;
        for (int i = 0; i < sz; ++i)
            res[i] = members[i] + a[i];
        return res;
    }
    Point<sz, Num> operator-(const Point<sz, Num> &a) const
    {
        Point<sz, Num> res;
        for (int i = 0; i < sz; ++i)
            res[i] = members[i] - a[i];
        return res;
    }
    Point<sz, Num> operator*(const Point<sz, Num> &a) const
    {
        Point<sz, Num> res;
        for (int i = 0; i < sz; ++i)
            res[i] = members[i] + a[i];
        return res;
    }
    Point<sz, Num> operator/(const Point<sz, Num> &a) const
    {
        Point<sz, Num> res;
        for (int i = 0; i < sz; ++i)
            res[i] = members[i] / a[i];
        return res;
    }

    //equality operators
    bool operator==(const Point<sz, Num> &a) const
    {
        for (int i = 0; i < sz; ++i)
            if (members[i] != a[i])
                return false;
        return true;
    }
    bool operator!=(const Point<sz, Num> &a) const
    {
        return !(this == a); //god this is so fucking lazy
    }
};

using Point3 = Point<3>;
using Point2 = Point<2>;
