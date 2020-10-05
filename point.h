#include "MiscHeaderFiles-master/generics.h"

template <typename T, size_t sz>
requires EqualityComparable<T> class rPoint
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
    rPoint()
    {
        for (size_t i = 0; i < sz; ++i)
        {
            members[i] = T();
        }
    }
    template <typename... Params>
    rPoint(Params... args)
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
    bool operator==(const rPoint<T, sz> &p) const
    {
        for (size_t i = 0; i < sz; ++i)
        {
            if (members[i] != p.members[i])
                return false;
        }
        return true;
    }
    bool operator!=(const rPoint<T, sz> &p) const
    {
        for (size_t i = 0; i < sz; ++i)
        {
            if (members[i] != p.members[i])
                return true;
        }
        return false;
    }
    friend std::ostream &operator<<(std::ostream &os, const rPoint<T, sz> &p)
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
struct Vec : public rPoint<T, sz>
{
    rPoint<T, sz> operator+(const rPoint<T, sz> &p) const
    {
        rPoint<T, sz> res;
        for (size_t i = 0; i < sz; ++i)
        {
            res.members[i] = this->members[i] + p.members[i];
        }
        return res;
    }
    rPoint<T, sz> operator-(const rPoint<T, sz> &p) const
    {
        rPoint<T, sz> res;
        for (size_t i = 0; i < sz; ++i)
        {
            res.members[i] = this->members[i] - p.members[i];
        }
        return res;
    }
    rPoint<T, sz> operator*(const rPoint<T, sz> &p) const
    {
        rPoint<T, sz> res;
        for (size_t i = 0; i < sz; ++i)
        {
            res.members[i] = this->members[i] * p.members[i];
        }
        return res;
    }
    rPoint<T, sz> operator/(const rPoint<T, sz> &p) const
    {
        rPoint<T, sz> res;
        for (size_t i = 0; i < sz; ++i)
        {
            res.members[i] = this->members[i] / p.members[i];
        }
        return res;
    }
};

//Vec2
template <typename T>
using Vec2 = Vec<T, 2>;

//Vec3
template <typename T>
using Vec3 = Vec<T, 3>;

//RGB
struct RGB : public Vec3<uint8_t>
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
struct RGBA : public Vec3<uint8_t>
{
    uint8_t &r = members[0];
    uint8_t &g = members[1];
    uint8_t &b = members[2];
    uint8_t &a = members[3];

    friend std::ostream &operator<<(std::ostream &os, const RGB &p)
    {
        return os << '(' << (int)p.r << ", " << (int)p.g << ", " << (int)p.b << ", " << (int)p.a << ')';
    }
};

using Point2i = Vec2<int>;
using Point3i = Vec3<int>;
using Point2f = Vec2<float>;
using Point3f = Vec3<float>;
