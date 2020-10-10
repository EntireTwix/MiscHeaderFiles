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
    Point(std::array<T, sz> &&args) : members(args) {}

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

    std::array<T, sz> Transform(std::function<T(const T &)> Func)
    {
        std::array<T, sz> res;
        for (size_t i = 0; i < sz; ++i)
        {
            res[i] = Func(members[i]);
        }
        return res;
    }
    std::array<T, sz> Transform(std::function<T(const T &, const T &)> Func, const std::array<T, sz> &p)
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
    std::array<T, sz> Transform(std::function<T(const T &, size_t)> Func)
    {
        std::array<T, sz> res;
        for (size_t i = 0; i < sz; ++i)
        {
            res[i] = Func(members[i], i);
        }
        return res;
    }
    std::array<T, sz> Transform(std::function<T(const T &, const T &, size_t)> Func, const std::array<T, sz> &p)
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

//generic Func

//Vec
template <typename T, size_t sz>
struct UnsafeVec : public Point<T, sz>
{
    UnsafeVec() = default;
    UnsafeVec(std::array<T, sz> &&args)
    {
        this->members = args;
    }

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
struct RGB : public UnsafeVec<uint_fast8_t, 3>
{
    RGB()
    {
        members[0] = members[1] = members[2] = 0;
    }

    RGB(uint_fast8_t R, uint_fast8_t G, uint_fast8_t B)
    {
        members[0] = R;
        members[1] = G;
        members[2] = B;
    }

    uint_fast8_t &r() { return members[0]; }
    uint_fast8_t &g() { return members[1]; }
    uint_fast8_t &b() { return members[2]; }

    uint_fast8_t r() const { return members[0]; }
    uint_fast8_t g() const { return members[1]; }
    uint_fast8_t b() const { return members[2]; }

    friend std::ostream &operator<<(std::ostream &os, const RGB &p)
    {
        return os << '(' << (int)p.members[0] << ", " << (int)p.members[1] << ", " << (int)p.members[2] << ')';
    }
};

//RGBA
struct RGBA : public RGB
{
    RGBA()
    {
        members[0] = members[1] = members[2] = members[3] = 0;
    }

    RGBA(uint_fast8_t R, uint_fast8_t G, uint_fast8_t B, uint_fast8_t A) : RGB(R, G, B)
    {
        members[3] = A;
    }

    uint_fast8_t &a() { return members[3]; }

    uint_fast8_t a() const { return members[3]; }

    friend std::ostream &operator<<(std::ostream &os, const RGBA &p)
    {
        return os << '(' << (int)p.members[0] << ", " << (int)p.members[1] << ", " << (int)p.members[2] << ", " << (int)p.members[3] << ')';
    }
};

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
