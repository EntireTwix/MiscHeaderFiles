#pragma once
#include "layers.h"
#include <vector>

template <typename... Params> 
class NueralNetwork
{
private:
    std::vector<Layer*> layers;

public:
    void AddLayer(Layer* l)
    {
        layers.push_back(l);
    }
};