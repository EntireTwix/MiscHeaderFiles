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
concept ArithmeticType = requires(T a, T b)
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

template <typename T>
concept BasicArithmeticType = requires(T a, T b)
{
    { a+b } -> std::convertible_to<T>;
    { a-b } -> std::convertible_to<T>;
    { a*b } -> std::convertible_to<T>;
    { a/b } -> std::convertible_to<T>;
};

template <BasicArithmeticType T>
std::vector<T> Softmax(const std::vector<T>& vec, T multiplier = 1)
{
    T sum{};
    std::vector<T> res;
    for(T x : vec) sum = sum + x;
    for(T x : vec) res.push_back(T((x/sum)*multiplier));
    return res;
}

template <EqualityComparable T>
bool Contains(const std::vector<T>& vec, T value)
{
    for( T t : vec ) if(t == value) return true;
    return false; 
}
