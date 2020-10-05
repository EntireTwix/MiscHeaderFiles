#include "generics.h"

template <typename T, size_t sz>
requires EqualityComparable<T> class Point
{
private:
    void Set(size_t index, const T &value)
    {
        members[index] = value;
    }

    template <typename... Params>
    void Set(size_t index, const T &head, Params... rest)
    {
        members[index] = head;
        Set(index + 1, rest...);
    }

protected:
    T members[sz];

public:
    Point()
    {
        for (size_t i = 0; i < sz; ++i)
        {
            members[i] = T();
        }
    }
    template <typename... Params>
    Point(Params... args)
    {
        if constexpr (sizeof...(args) != sz)
            throw std::invalid_argument("amount of args must match point size");

        Set(0, args...);
    }

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
};

//generic Func

//Vec
template <typename T, size_t sz>
struct Vec : public Point<T, sz>
{
    Point<T, sz> operator+(const Point<T, sz> &p) const
    {
        Point<T, sz> res;
        for (size_t i = 0; i < sz; ++i)
        {
            res.members[i] = this->members[i] + p.members[i];
        }
        return res;
    }
    Point<T, sz> operator-(const Point<T, sz> &p) const
    {
        Point<T, sz> res;
        for (size_t i = 0; i < sz; ++i)
        {
            res.members[i] = this->members[i] - p.members[i];
        }
        return res;
    }
    Point<T, sz> operator*(const Point<T, sz> &p) const
    {
        Point<T, sz> res;
        for (size_t i = 0; i < sz; ++i)
        {
            res.members[i] = this->members[i] * p.members[i];
        }
        return res;
    }
    Point<T, sz> operator/(const Point<T, sz> &p) const
    {
        Point<T, sz> res;
        for (size_t i = 0; i < sz; ++i)
        {
            res.members[i] = this->members[i] / p.members[i];
        }
        return res;
    }
};

//SafeVec
template <typename T, size_t sz>
requires ArithmeticOperators<T> using SafeVec = Vec<T, sz>;

//Vec2
template <typename T>
using Vec2 = SafeVec<T, 2>;

//Vec3
template <typename T>
using Vec3 = SafeVec<T, 3>;

//RGB
struct RGB : public Vec<uint8_t, 3>
{
    uint8_t &r = members[0];
    uint8_t &g = members[1];
    uint8_t &b = members[2];

    friend std::ostream &operator<<(std::ostream &os, const RGB &p)
    {
        return os << '(' << (int)p.r << ", " << (int)p.g << ", " << (int)p.b << ')';
    }
};

//RGBA
struct RGBA : public Vec<uint8_t, 4>
{
    uint8_t &r = members[0];
    uint8_t &g = members[1];
    uint8_t &b = members[2];
    uint8_t &a = members[3];

    friend std::ostream &operator<<(std::ostream &os, const RGBA &p)
    {
        return os << '(' << (int)p.r << ", " << (int)p.g << ", " << (int)p.b << ", " << (int)p.a << ')';
    }
};

using Point2i = Vec2<int>;
using Point3i = Vec3<int>;
using Point2f = Vec2<float>;
using Point3f = Vec3<float>;
