#pragma once
#include <algorithm>
#include <vector>


template <typename T>
class ISortable {
    virtual bool operator<(const T& instance) = 0;
    virtual bool operator>(const T& instance) = 0;
};
