#pragma once
#include <stdexcept>
#include <ostream>
#include <functional>
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
    Type At(size_t x, size_t y) const;

    constexpr size_t SizeX() const;
    constexpr size_t SizeY() const;
    constexpr size_t Area() const;

    Type *begin()
    {
        return &members[0];
    }
    Type *end()
    {
        return &members[width * height];
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
inline Type &Mat<Type>::At(size_t x, size_t y) //indexing matrix
{
    if ((x >= SizeX()) || (y >= SizeY()))
        throw std::out_of_range("At: out of range, " + std::to_string(x) + ' ' + std::to_string(y));
    return members[(y * SizeX()) + x];
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
inline Mat<Type>::~Mat()
{
    delete[] members;
}
