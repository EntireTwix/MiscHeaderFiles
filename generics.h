#pragma once
#include <vector>
#include <concepts>

template <typename T>
concept EqualityComparable = requires(T a, T b) 
{
    { a == b } -> std::convertible_to<bool>;
    { a != b } -> std::convertible_to<bool>;
};

template <typename T>
concept IsArithmeticType = requires(T a, T b)
{
    { a++ } -> std::convertible_to<T>;
    { a-- } -> std::convertible_to<T>;
    { a+b } -> std::convertible_to<T>;
    { a-b } -> std::convertible_to<T>;
    { a*b } -> std::convertible_to<T>;
    { a/b } -> std::convertible_to<T>;
    { a%b } -> std::convertible_to<T>;
    { a<b } -> std::convertible_to<bool>;
    { a<=b } -> std::convertible_to<bool>;
    { a>b } -> std::convertible_to<bool>;
    { a>=b } -> std::convertible_to<bool>;
    { a==b } -> std::convertible_to<bool>;
    { a!=b } -> std::convertible_to<bool>;
};

template <EqualityComparable T>
bool Contains(const std::vector<T>& vec, T value)
{
    for( T t : vec ) if(t == value) return true;
    return false; 
}