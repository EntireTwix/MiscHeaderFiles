#pragma once
#include "mat.h"
#include <vector>

//use std::uniform_real_distribution<>

class NeuralNetwork
{
private:
    Mat<double>* layers = nullptr;
    size_t size = 0; //possibly irrelevant variable
public: 
    template <typename... Params>
    NeuralNetwork(size_t first, Params ...p)
    {
        size = ((sizeof...(p))*2)+1; //possibly irrelevant
        layers = new Mat<double>[size];
        layers[0] = Mat<double>(first, 1); 
        AddLayer(1, p...);
    }

    template <typename... Params, Number T>
    void AddLayer(size_t loc, T sz, Params... p)
    {
        layers[loc] = Mat<double>(sz, layers[loc-1].sizeX()); //weight
        layers[loc+1] = Mat<double>(sz, 1); //biases
        loc+=2;
        if constexpr(sizeof...(p)) AddLayer(loc, p...);
    }

    ~NeuralNetwork()
    {
        delete[] layers;
    }
};