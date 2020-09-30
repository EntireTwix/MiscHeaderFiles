#pragma once
#include <vector>
#include <concepts>
#include <type_traits>

//concepts 
template <typename T>
concept Func = requires { T(); };

template <typename T>
concept EqualityComparable = requires(T a, T b) 
{
    { a == b } -> std::same_as<bool>;
    { a != b } -> std::same_as<bool>;
};

template <typename T>
concept UnaryOperators = requires(T a)
{
    { a++ } -> std::same_as<T>;
    { a-- } -> std::same_as<T>;
};

template <typename T, typename T2 = T, typename T3 = T>
concept ArithmeticOperators = requires(T a, T2 b)
{
    { a+b } -> std::same_as<T3>;
    { a-b } -> std::same_as<T3>;
    { a*b } -> std::same_as<T3>;
    { a/b } -> std::same_as<T3>;
};

template <typename T>
concept RelationalOperators = requires(T a, T b)
{
    { a<b } -> std::same_as<bool>;
    { a<=b } -> std::same_as<bool>;
    { a>b } -> std::same_as<bool>;
    { a>=b } -> std::same_as<bool>;

    requires EqualityComparable<T>;
};

template <typename T>
concept Number = std::convertible_to<T, int>;

//lifted functions

template <ArithmeticOperators T>
std::vector<double> Softmax(const std::vector<T>& vec, double multiplier = 1)
{
    T sum{};
    std::vector<double> res;
    
    if constexpr(std::is_fundamental<T>::value) for(T x : vec) sum = sum + x;
    else for(const T& x : vec) sum = sum + x;
    
    if constexpr(std::is_fundamental<T>::value) for(T x : vec) res.push_back((double)((x/sum)*multiplier));
    else for(const T& x : vec) res.push_back((double)((x/sum)*multiplier));

    return res;
}

//to-do replace vector with sequence concept
