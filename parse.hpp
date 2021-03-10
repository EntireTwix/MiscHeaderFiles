#pragma once
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

//File I/O
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
inline std::vector<std::string> ReadFileSimple(const std::string &file, char delim, T &&func)
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

//string Split
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
inline std::vector<std::string> Split(const std::string &inp, char delim, T &&func)
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
