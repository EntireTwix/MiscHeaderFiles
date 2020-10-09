#pragma once
#include <stdexcept>
#include <ostream>
#include <functional>

template <typename Type = float>
class Mat
{
private:
    Type *members = nullptr;
    size_t sizeX = 0, sizeY = 0;

public:
    Mat() = default;
    explicit Mat(size_t x, size_t y);
    explicit Mat(size_t w, size_t h, std::initializer_list<Type> membs);
    Mat(const Mat &mat);
    Mat(Mat &&mat);
    Mat operator=(const Mat &mat);
    Mat operator=(Mat &&mat);

    Type &At(size_t x, size_t y);
    Type At(size_t x, size_t y) const;

    size_t SizeX() const;
    size_t SizeY() const;
    size_t Area() const;

    Type *begin()
    {
        return &members[0];
    }
    Type *end()
    {
        return &members[sizeX * sizeY];
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

    ~Mat();
};

template <typename Type>
inline Mat<Type>::Mat(size_t x, size_t y) : sizeX(x), sizeY(y)
{
    members = new Type[sizeX * sizeY]{Type()};
}

//this constructor is unsafe and slow, is meant for loading from save
template <typename Type>
inline Mat<Type>::Mat(size_t w, size_t h, std::initializer_list<Type> arr) : sizeX(w), sizeY(h)
{
    members = new float[w * h];
    for (size_t i = 0; i < arr.size(); ++i)
    {
        members[i] = *(arr.begin() + i);
    }
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
inline size_t Mat<Type>::Area() const
{
    return sizeY * sizeX;
}

template <typename Type>
inline Mat<Type>::~Mat()
{
    delete[] members;
}
