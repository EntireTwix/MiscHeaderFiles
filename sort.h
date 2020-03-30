#pragma once
#include <algorithm>
#include <vector>

//always add as inhereted by doing virtual ISortable so that it wont break
template <typename T>
class ISortable {
    virtual bool operator<(const T& instance) = 0;
};
