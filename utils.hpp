#pragma once
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

template <typename T>
auto CombineBits(T val)
{
        return val;
}

template <typename T, typename... Bits>
auto CombineBits(T head, Bits... vars)
{
        return (head << sizeof(T)) + CombineBits(vars...);
}

inline std::vector<std::string> ReadFileSimple(const std::string &file, char delim)
{
    std::ifstream temp(file);
    std::vector<std::string> res;
    std::string line;
    if (temp.is_open())
    {
        while (std::getline(temp, line, delim))
        {
            res.push_back(line);
        }
        temp.close();
    }
    return res;
}

template <typename T>
inline std::vector<std::string> ReadFileSimple(const std::string &file, char delim, const T &func)
{
    std::ifstream temp(file);
    std::vector<std::string> res;
    std::string line;
    if (temp.is_open())
    {
        while (std::getline(temp, line, delim))
        {
            func(line);
            res.push_back(line);
        }
        temp.close();
    }
    return res;
}

inline std::vector<std::string> Split(const std::string &inp, char delim)
{
    std::stringstream ss(inp);
    std::vector<std::string> res;
    std::string line;
    while (std::getline(ss, line, delim))
    {
        res.push_back(line);
    }
    return res;
}

template <typename T>
inline std::vector<std::string> Split(const std::string &inp, char delim, const T &func)
{
    std::stringstream ss(inp);
    std::vector<std::string> res;
    std::string line;
    while (std::getline(ss, line, delim))
    {
        func(line);
        res.push_back(line);
    }
    return res;
}
