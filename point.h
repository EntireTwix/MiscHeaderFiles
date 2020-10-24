#pragma once
#include "generics.h"
#include <stdexcept>
#include <ostream>
#include <functional>
#include <array>

template <typename T, size_t sz>
requires EqualityComparable<T> class Point
{
protected:
    std::array<T, sz> members;

public:
    Point() = default;
    Point(const std::array<T, sz> &arr) : members(arr) {}
    explicit Point(auto head, auto &&... args) : members{head, args...} { static_assert(sizeof...(args) + 1 == sz, "args initilizing point must be the same size"); }

    T &operator[](size_t index)
    {
        if (index >= sz)
            throw std::out_of_range("index given is too big");
        return members[index];
    }
    T operator[](size_t index) const
    {
        if (index >= sz)
            throw std::out_of_range("index given is too big");
        return members[index];
    }
    bool operator==(const Point<T, sz> &p) const
    {
        for (size_t i = 0; i < sz; ++i)
        {
            if (members[i] != p.members[i])
                return false;
        }
        return true;
    }
    bool operator!=(const Point<T, sz> &p) const
    {
        for (size_t i = 0; i < sz; ++i)
        {
            if (members[i] != p.members[i])
                return true;
        }
        return false;
    }
    friend std::ostream &operator<<(std::ostream &os, const Point<T, sz> &p)
    {
        os << '(';
        for (size_t i = 0; i < sz; ++i)
        {
            os << p.members[i] << (i + 1 == sz ? "" : ", ");
        }
        return os << ')';
    }

    std::array<T, sz> Transform(std::function<T(const T &)> Func) const
    {
        std::array<T, sz> res;
        for (size_t i = 0; i < sz; ++i)
        {
            res[i] = Func(members[i]);
        }
        return res;
    }
    std::array<T, sz> Transform(std::function<T(const T &, const T &)> Func, const std::array<T, sz> &p) const
    {
        std::array<T, sz> res;
        for (size_t i = 0; i < sz; ++i)
        {
            res[i] = Func(members[i], p[i]);
        }
        return res;
    }
    void ApplyFunc(std::function<void(T &)> Func)
    {
        for (T &e : members)
        {
            Func(e);
        }
    }

    //cord variants
    std::array<T, sz> Transform(std::function<T(const T &, size_t)> Func) const
    {
        std::array<T, sz> res;
        for (size_t i = 0; i < sz; ++i)
        {
            res[i] = Func(members[i], i);
        }
        return res;
    }
    std::array<T, sz> Transform(std::function<T(const T &, const T &, size_t)> Func, const std::array<T, sz> &p) const
    {
        std::array<T, sz> res;
        for (size_t i = 0; i < sz; ++i)
        {
            res[i] = Func(members[i], p[i], i);
        }
        return res;
    }
    void ApplyFunc(std::function<void(T &, size_t)> Func)
    {
        for (size_t i = 0; i < sz; ++i)
        {
            Func(members[i], i);
        }
    }

    constexpr size_t size() const
    {
        return sz;
    }
};

//Vec
template <typename T, size_t sz>
struct UnsafeVec : public Point<T, sz>
{
    UnsafeVec() = default;
    UnsafeVec(const std::array<T, sz> &arr) : Point<T, sz>(arr) {}
    explicit UnsafeVec(auto head, auto &&... args) : Point<T, sz>(head, args...) {}

    std::array<T, sz> operator+(const UnsafeVec<T, sz> &p) const
    {
        std::array<T, sz> res;
        for (size_t i = 0; i < sz; ++i)
        {
            res[i] = this->members[i] + p[i];
        }
        return res;
    }
    std::array<T, sz> operator-(const UnsafeVec<T, sz> &p) const
    {
        std::array<T, sz> res;
        for (size_t i = 0; i < sz; ++i)
        {
            res[i] = this->members[i] - p[i];
        }
        return res;
    }
    UnsafeVec<T, sz> operator*(const UnsafeVec<T, sz> &p) const
    {
        std::array<T, sz> res;
        for (size_t i = 0; i < sz; ++i)
        {
            res[i] = this->members[i] * p[i];
        }
        return res;
    }
    UnsafeVec<T, sz> operator/(const UnsafeVec<T, sz> &p) const
    {
        std::array<T, sz> res;
        for (size_t i = 0; i < sz; ++i)
        {
            res[i] = this->members[i] / p[i];
        }
        return res;
    }
};

//SafeVec
template <typename T, size_t sz>
requires ArithmeticOperators<T> using SafeVec = UnsafeVec<T, sz>;

//RGB
using RGB = UnsafeVec<uint8_t, 3>;

//RGBA
using RGBA = UnsafeVec<uint8_t, 4>;

template <typename T, size_t sz>
using Vec = SafeVec<T, sz>;

template <typename T>
using Vec2 = SafeVec<T, 2>;

template <typename T>
using Vec3 = SafeVec<T, 3>;

using Point2i = Vec2<int>;
using Point3i = Vec3<int>;
using Point2f = Vec2<float>;
using Point3f = Vec3<float>;
