#pragma once
#include <stdexcept>
#include <ostream>
#include <functiona;>
#include <array>

template <typename Type = float, size_t width, size_t height>
class Mat
{
private:
    std::array<Type, width * height> members;

public:
    Mat() = default;
    explicit Mat(std::array<Type, width * height> members) members(members) {}

    Type &At(size_t x, size_t y);
    Type *AtP(size_t x, size_t y);
    Type At(size_t x, size_t y) const;

    constexpr size_t SizeX() const;
    constexpr size_t SizeY() const;
    constexpr size_t Area() const;

    Mat Dot(const Mat &mat) const;
    Mat VecMult(const Mat &mat) const;
    Mat DoubleVecMult(const Mat &mat) const;
    Mat Transpose() const;
    Mat CompoundingVecMult(const Mat &mat) const;

    Mat operator+(const Mat &mat) const;
    Mat operator-(const Mat &mat) const;
    Mat operator*(const Mat &mat) const;
    Mat operator/(const Mat &mat) const;

    Type *begin()
    {
        return &members[0];
    }
    Type *end()
    {
        return &members[width * height];
    }

    friend std::ostream &operator<<(std::ostream &os, const Mat &mat)
    {
        for (size_t i = 0; i < mat.SizeY(); ++i)
        {
            for (size_t j = 0; j < mat.SizeX(); ++j)
                os << mat.At(j, i) << ' ';
            os << '\n';
        }
        return os;
    }

    std::string ToString() const
    {
        std::string res;
        for (size_t i = 0; i < SizeY(); ++i)
        {
            for (size_t j = 0; j < SizeX(); ++j)
            {
                res += std::to_string(At(j, i)) + ' ';
            }
            res += '\n';
        }
        return res;
    }

    std::string Save() const
    {
        std::string res = "";
        res += std::to_string(SizeX()) + ' ' + std::to_string(SizeY()) + " {";
        for (Type *start = &members[0]; start != &members[SizeX() * SizeY()]; ++start)
        {
            res += std::to_string(*start);
            if (start != &members[SizeX() * SizeY()] - 1)
            {
                res += ", ";
            }
        }
        res += "};";
        return res;
    }

    Mat ApplyFunc(std::function<Type(Type)> Func)
    {
        Mat<Type> res(SizeX(), SizeY());
        for (int i = 0; i < SizeY(); ++i)
        {
            for (int j = 0; j < SizeX(); ++j)
            {
                res.At(j, i) = Func(At(j, i));
            }
        }
        return res;
    }
    Mat ApplyFunc(std::function<Type(Type, Type)> Func, const Mat<Type> &p)
    {
        Mat<Type> res(SizeX(), SizeY());
        for (int i = 0; i < SizeY(); ++i)
        {
            for (int j = 0; j < SizeX(); ++j)
            {
                res.At(j, i) = Func(At(j, i), p.At(j, i));
            }
        }
        return res;
    }

    void ApplyFunc(std::function<void(Type &)> Func)
    {
        for (Type &e : members)
        {
            Func(e);
        }
    }

    //cord variants
    Mat ApplyFunc(std::function<Type(Type, size_t, size_t)> Func)
    {
        Mat<Type> res(SizeX(), SizeY());
        for (int i = 0; i < SizeY(); ++i)
        {
            for (int j = 0; j < SizeX(); ++j)
            {
                res.At(j, i) = Func(At(j, i), j, i);
            }
        }
        return res;
    }
    Mat ApplyFunc(std::function<Type(Type, Type, size_t, size_t)> Func, const Mat<Type> &p)
    {
        Mat<Type> res(SizeX(), SizeY());
        for (int i = 0; i < SizeY(); ++i)
        {
            for (int j = 0; j < SizeX(); ++j)
            {
                res.At(j, i) = Func(At(j, i), p.At(j, i), j, i);
            }
        }
        return res;
    }

    void ApplyFunc(std::function<void(Type &, size_t, size_t)> Func)
    {
        for (int i = 0; i < SizeY(); ++i)
        {
            for (int j = 0; j < SizeX(); ++j)
            {
                Func(At(j, i), j, i);
            }
        }
    }

    ~Mat();
};

template <typename Type>
inline Type &Mat<Type>::At(size_t x, size_t y) //indexing matrix
{
    if ((x >= SizeX()) || (y >= SizeY()))
        throw std::out_of_range("At: out of range, " + std::to_string(x) + ' ' + std::to_string(y));
    return members[(y * SizeX()) + x];
}

template <typename Type>
inline Type *Mat<Type>::AtP(size_t x, size_t y)
{
    if ((x >= SizeX()) || (y >= SizeY()))
        throw std::out_of_range("At: out of range, " + std::to_string(x) + ' ' + std::to_string(y));
    return &members[(y * SizeX()) + x];
}

template <typename Type>
inline Type Mat<Type>::At(size_t x, size_t y) const //indexing matrix
{
    if ((x >= SizeX()) || (y >= SizeY()))
        throw std::out_of_range("At: out of range, " + std::to_string(x) + ' ' + std::to_string(y));
    return members[(y * SizeX()) + x];
}

template <typename Type>
constexpr inline size_t Mat<Type>::SizeX() const
{
    return width;
}

template <typename Type>
constexpr inline size_t Mat<Type>::SizeY() const
{
    return height;
}

template <typename Type>
constexpr inline size_t Mat<Type>::Area() const
{
    return width * height;
}

template <typename Type>
inline Mat<Type> Mat<Type>::Dot(const Mat<Type> &mat) const
{
    if (SizeY() != 1)
        throw std::invalid_argument("input must be a vector for weight layer");
    if (SizeX() != mat.SizeY())
        throw std::invalid_argument("y of input must equal x of weight");

    Mat res(mat.SizeX(), SizeY()); //product dimensions are the x of both mats
    Type amount;
    for (size_t i = 0; i < SizeY(); ++i)         //for each y on the A matrix
        for (size_t j = 0; j < mat.SizeX(); ++j) //for each x on the B matrix
        {
            amount = 0;
            for (int k = 0; k < SizeX(); ++k) //is equivelant to mat.SizeY()()
                amount += At(k, i) * mat.At(j, k);
            res.At(j, i) = amount;
        }
    return res;
}

template <typename Type>
inline Mat<Type> Mat<Type>::VecMult(const Mat<Type> &mat) const
{
    Mat res(SizeX(), SizeY());
    if (mat.SizeX() != SizeY())
        throw std::invalid_argument("SizeX() of the argument must meet the SizeY() of the calling object");
    for (size_t i = 0; i < SizeY(); ++i)
        for (size_t j = 0; j < SizeX(); ++j)
            res.At(j, i) = At(j, i) * mat.At(i, 0);
    return res;
}

template <typename Type>
inline Mat<Type> Mat<Type>::DoubleVecMult(const Mat<Type> &mat) const
{
    Mat res(SizeX(), mat.SizeX());
    if (mat.SizeY() != SizeY())
        throw std::invalid_argument("SizeY() of both must be 1");
    for (size_t i = 0; i < mat.SizeX(); ++i)
        for (size_t j = 0; j < SizeX(); ++j)
            res.At(j, i) = At(j, 0) * mat.At(i, 0);
    return res;
}

template <typename Type>
inline Mat<Type> Mat<Type>::Transpose() const
{
    Mat res(SizeY(), SizeX());
    for (size_t i = 0; i < SizeY(); ++i)
        for (size_t j = 0; j < SizeX(); ++j)
            res.At(i, j) = At(j, i);
    return res;
}

template <typename Type>
inline Mat<Type> Mat<Type>::operator+(const Mat<Type> &mat) const
{
    if ((mat.SizeX() != SizeX()) || (mat.SizeY() != SizeY()))
        throw std::invalid_argument("operator+: parameter must match the dimensions of this");
    Mat res(SizeX(), SizeY());
    for (size_t i = 0; i < SizeY(); ++i)
        for (size_t j = 0; j < SizeX(); ++j)
            res.At(j, i) = At(j, i) + mat.At(j, i);
    return res;
}

template <typename Type>
inline Mat<Type> Mat<Type>::operator-(const Mat<Type> &mat) const
{
    if ((mat.SizeX() != SizeX()) || (mat.SizeY() != SizeY()))
        throw std::invalid_argument("operator-: parameter must match the dimensions of this");
    Mat res(SizeX(), SizeY());
    for (size_t i = 0; i < SizeY(); ++i)
        for (size_t j = 0; j < SizeX(); ++j)
            res.At(j, i) = At(j, i) - mat.At(j, i);
    return res;
}

template <typename Type>
inline Mat<Type> Mat<Type>::operator*(const Mat<Type> &mat) const
{
    if ((mat.SizeX() != SizeX()) || (mat.SizeY() != SizeY()))
        throw std::invalid_argument("operator*: parameter must match the dimensions of this");
    Mat res(SizeX(), SizeY());
    for (size_t i = 0; i < SizeY(); ++i)
        for (size_t j = 0; j < SizeX(); ++j)
            res.At(j, i) = At(j, i) * mat.At(j, i);
    return res;
}

template <typename Type>
inline Mat<Type> Mat<Type>::operator*(const Mat<Type> &mat) const
{
    if ((mat.SizeX() != SizeX()) || (mat.SizeY() != SizeY()))
        throw std::invalid_argument("operator*: parameter must match the dimensions of this");
    Mat res(SizeX(), SizeY());
    for (size_t i = 0; i < SizeY(); ++i)
        for (size_t j = 0; j < SizeX(); ++j)
            res.At(j, i) = At(j, i) / mat.At(j, i);
    return res;
}

template <typename Type>
inline Mat<Type>::~Mat()
{
    delete[] members;
}
