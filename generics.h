#pragma once
#include <vector>
#include <concepts>

//concepts 

template <typename T>
concept EqualityComparable = requires(T a, T b) 
{
    { a == b } -> std::convertible_to<bool>;
    { a != b } -> std::convertible_to<bool>;
};

template <typename T>
concept UnaryOperators = requires(T a)
{
    { a++ } -> std::convertible_to<T>;
    { a-- } -> std::convertible_to<T>;
};

template <typename T, typename T2 = T, typename T3 = T>
concept ArithmeticOperators = requires(T a, T2 b)
{
    { a+b } -> std::convertible_to<T3>;
    { a-b } -> std::convertible_to<T3>;
    { a*b } -> std::convertible_to<T3>;
    { a/b } -> std::convertible_to<T3>;
};

template <typename T>
concept RelationalOperators = requires(T a, T b)
{
    { a<b } -> std::convertible_to<bool>;
    { a<=b } -> std::convertible_to<bool>;
    { a>b } -> std::convertible_to<bool>;
    { a>=b } -> std::convertible_to<bool>;
    { a==b } -> std::convertible_to<bool>;
    { a!=b } -> std::convertible_to<bool>;
};

//lifted functions

template <ArithmeticOperators T>
std::vector<double> Softmax(const std::vector<T>& vec, double multiplier = 1)
{
    T sum{};
    std::vector<double> res;
    for(T x : vec) sum = sum + x;
    for(T x : vec) res.push_back((double)((x/sum)*multiplier));
    return res;
}

template <EqualityComparable T>
bool Contains(const std::vector<T>& vec, T value)
{
    for( T t : vec ) if(t == value) return true;
    return false; 
}
