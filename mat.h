#pragma once
#include <stdexcept>
#include <ostream>
#include "generics.h"

template <typename Type = float>
class Mat
{
private:
    Type *members = nullptr;
    size_t sizeX = 0, sizeY = 0;

public:
    Mat() = default;
    explicit Mat(size_t x, size_t y);
    Mat(const Mat &mat);
    Mat(Mat &&mat);
    Mat operator=(const Mat &mat);
    Mat operator=(Mat &&mat);

    Type &At(size_t x, size_t y);
    Type *AtP(size_t x, size_t y);
    Type At(size_t x, size_t y) const;

    size_t SizeX() const;
    size_t SizeY() const;

    Mat Dot(const Mat &mat) const;
    Mat VecMult(const Mat &mat) const;
    Mat DoubleVecMult(const Mat &mat) const;
    Mat Transpose() const;
    Mat CompoundingVecMult(const Mat &mat) const;

    template <typename... Params>
    void Func(void (*func)(Type &), Params... p);
    void Func(void (*func)(Type &));

    template <typename... Params>
    Mat Func(Type (*func)(Type), Params... p) const;
    Mat Func(Type (*func)(Type)) const;
    Mat Func(const Mat &mat, Type (*func)(Type, Type)) const;

    Mat operator+(const Mat &mat) const;
    Mat operator-(const Mat &mat) const;
    Mat operator*(const Mat &mat) const;

    Type *begin()
    {
        return &members[0];
    }
    Type *end()
    {
        return &members[sizeX * sizeY];
    }

    friend std::ostream &operator<<(std::ostream &os, const Mat &mat)
    {
        for (size_t i = 0; i < mat.SizeY(); ++i)
        {
            for (size_t j = 0; j < mat.SizeX(); ++j)
                os << (Type)mat.At(j, i) << ' ';
            os << '\n';
        }
        return os;
    }

    ~Mat();
};

template <typename Type>
inline Mat<Type>::Mat(size_t x, size_t y) : sizeX(x), sizeY(y)
{
    members = new Type[sizeX * sizeY]{Type()};
}

template <typename Type>
inline Mat<Type>::Mat(const Mat<Type> &mat)
{
    sizeX = mat.sizeX;
    sizeY = mat.sizeY;
    members = new Type[sizeX * sizeY];

    for (size_t i = 0; i < sizeY; ++i)
        for (size_t j = 0; j < sizeX; ++j)
            At(j, i) = mat.At(j, i);
}

template <typename Type>
inline Mat<Type>::Mat(Mat<Type> &&mat)
{
    sizeX = mat.sizeX;
    sizeY = mat.sizeY;
    members = std::move(mat.members);
    mat.members = nullptr;
}

template <typename Type>
inline Mat<Type> Mat<Type>::operator=(const Mat<Type> &mat)
{
    sizeX = mat.sizeX;
    sizeY = mat.sizeY;
    members = new Type[sizeX * sizeY];
    for (size_t i = 0; i < sizeY; ++i)
        for (size_t j = 0; j < sizeX; ++j)
            At(j, i) = mat.At(j, i);
    return *this;
}

template <typename Type>
inline Mat<Type> Mat<Type>::operator=(Mat<Type> &&mat)
{
    sizeX = mat.sizeX;
    sizeY = mat.sizeY;
    members = std::move(mat.members);
    mat.members = nullptr;
    return *this;
}

template <typename Type>
inline Type &Mat<Type>::At(size_t x, size_t y) //indexing matrix
{
    if ((x >= sizeX) || (y >= sizeY))
        throw std::out_of_range("At: out of range, " + std::to_string(x) + ' ' + std::to_string(y));
    return members[(y * sizeX) + x];
}

template <typename Type>
inline Type *Mat<Type>::AtP(size_t x, size_t y)
{
    if ((x >= sizeX) || (y >= sizeY))
        throw std::out_of_range("At: out of range, " + std::to_string(x) + ' ' + std::to_string(y));
    return &members[(y * sizeX) + x];
}

template <typename Type>
inline Type Mat<Type>::At(size_t x, size_t y) const //indexing matrix
{
    if ((x >= sizeX) || (y >= sizeY))
        throw std::out_of_range("At: out of range, " + std::to_string(x) + ' ' + std::to_string(y));
    return members[(y * sizeX) + x];
}

template <typename Type>
inline size_t Mat<Type>::SizeX() const
{
    return sizeX;
}

template <typename Type>
inline size_t Mat<Type>::SizeY() const
{
    return sizeY;
}

template <typename Type>
inline Mat<Type> Mat<Type>::Dot(const Mat<Type> &mat) const
{
    if (sizeY != 1)
        throw std::invalid_argument("input must be a vector for weight layer");
    if (sizeX != mat.sizeY)
        throw std::invalid_argument("y of input must equal x of weight");

    Mat res(mat.sizeX, sizeY); //product dimensions are the x of both mats
    Type amount;
    for (size_t i = 0; i < sizeY; ++i)         //for each y on the A matrix
        for (size_t j = 0; j < mat.sizeX; ++j) //for each x on the B matrix
        {
            amount = 0;
            for (int k = 0; k < sizeX; ++k) //is equivelant to mat.sizeY()
                amount += At(k, i) * mat.At(j, k);
            res.At(j, i) = amount;
        }
    return res;
}

template <typename Type>
inline Mat<Type> Mat<Type>::VecMult(const Mat<Type> &mat) const
{
    Mat res(sizeX, sizeY);
    if (mat.sizeX != sizeY)
        throw std::invalid_argument("sizeX of the argument must meet the sizeY of the calling object");
    for (size_t i = 0; i < sizeY; ++i)
        for (size_t j = 0; j < sizeX; ++j)
            res.At(j, i) = At(j, i) * mat.At(i, 0);
    return res;
}

template <typename Type>
inline Mat<Type> Mat<Type>::DoubleVecMult(const Mat<Type> &mat) const
{
    Mat res(sizeX, mat.sizeX);
    if (mat.sizeY != sizeY)
        throw std::invalid_argument("sizeY of both must be 1");
    for (size_t i = 0; i < mat.sizeX; ++i)
        for (size_t j = 0; j < sizeX; ++j)
            res.At(j, i) = At(j, 0) * mat.At(i, 0);
    return res;
}

template <typename Type>
inline Mat<Type> Mat<Type>::Transpose() const
{
    Mat res(sizeY, sizeX);
    for (size_t i = 0; i < sizeY; ++i)
        for (size_t j = 0; j < sizeX; ++j)
            res.At(i, j) = At(j, i);
    return res;
}

template <typename Type>
template <typename... Params>
inline void Mat<Type>::Func(void (*func)(Type &), Params... p)
{
    for (Type *start = begin(); start != end(); ++start)
        func(*start, p...);
}

template <typename Type>
void Mat<Type>::Func(void (*func)(Type &))
{
    for (Type *start = begin(); start != end(); ++start)
        func(*start);
}

template <typename Type>
template <typename... Params>
inline Mat<Type> Mat<Type>::Func(Type (*func)(Type), Params... p) const
{
    Mat res(SizeX(), SizeY());
    for (size_t i = 0; i < sizeY; ++i)
        for (size_t j = 0; j < sizeX; ++j)
            res.At(j, i) = func(At(j, i), p...);
    return res;
}

template <typename Type>
inline Mat<Type> Mat<Type>::Func(Type (*func)(Type)) const
{
    Mat res(SizeX(), SizeY());
    for (size_t i = 0; i < sizeY; ++i)
        for (size_t j = 0; j < sizeX; ++j)
            res.At(j, i) = func(At(j, i));
    return res;
}

template <typename Type>
inline Mat<Type> Mat<Type>::Func(const Mat<Type> &mat, Type (*func)(Type, Type)) const
{
    if ((mat.sizeX != sizeX) || (mat.sizeY != sizeY))
        throw std::invalid_argument("matrices must be the same dimensions");
    Mat res(SizeX(), SizeY());
    for (size_t i = 0; i < sizeY; ++i)
        for (size_t j = 0; j < sizeX; ++j)
            res.At(j, i) = func(At(j, i), mat.At(j, i));
    return res;
}

template <typename Type>
inline Mat<Type> Mat<Type>::operator+(const Mat<Type> &mat) const
{
    if ((mat.SizeX() != SizeX()) || (mat.SizeY() != SizeY()))
        throw std::invalid_argument("operator+: parameter must match the dimensions of this");
    Mat res(SizeX(), SizeY());
    for (size_t i = 0; i < sizeY; ++i)
        for (size_t j = 0; j < sizeX; ++j)
            res.At(j, i) = At(j, i) + mat.At(j, i);
    return res;
}

template <typename Type>
inline Mat<Type> Mat<Type>::operator-(const Mat<Type> &mat) const
{
    if ((mat.SizeX() != SizeX()) || (mat.SizeY() != SizeY()))
        throw std::invalid_argument("operator-: parameter must match the dimensions of this");
    Mat res(SizeX(), SizeY());
    for (size_t i = 0; i < sizeY; ++i)
        for (size_t j = 0; j < sizeX; ++j)
            res.At(j, i) = At(j, i) - mat.At(j, i);
    return res;
}

template <typename Type>
inline Mat<Type> Mat<Type>::operator*(const Mat<Type> &mat) const
{
    if ((mat.SizeX() != SizeX()) || (mat.SizeY() != SizeY()))
        throw std::invalid_argument("operator*: parameter must match the dimensions of this");
    Mat res(SizeX(), SizeY());
    for (size_t i = 0; i < sizeY; ++i)
        for (size_t j = 0; j < sizeX; ++j)
            res.At(j, i) = At(j, i) * mat.At(j, i);
    return res;
}

template <typename Type>
inline Mat<Type>::~Mat()
{
    delete[] members;
}

//make initilizer list constructor
