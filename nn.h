#pragma once
#include <memory>
#include <thread>
#include "mat.h"

//use std::uniform_real_distribution<>

class NeuralNetwork
{
private:
    Mat<double>* layers = nullptr;
    size_t size = 0; //possibly irrelevant variable
    Mat<double> ForwardProp() const
    {
        std::unique_ptr< Mat<double>[] > currentMat = std::make_unique< Mat<double>[]>(size); //smart pointers gang
        for(size_t i = 0; i < size; ++i)
        {
            currentMat[i] = Mat<double>(layers[i].sizeX(), layers[i].sizeY());
            for(size_t j = 0; j < layers[i].sizeY(); ++j)
            for(size_t k = 0; k < layers[i].sizeX(); ++k)
                currentMat[i].at(j, k) = layers[i].at(j, k);
        }
        //do stuff
        return currentMat[size-1];
    }
public: 
    template <typename... Params>
    NeuralNetwork(size_t first, Params ...p)
    {
        size = ((sizeof...(p))*2)+1; //possibly irrelevant
        layers = new Mat<double>[size];
        layers[0] = Mat<double>(first, 1); 
        AddLayer(1, p...);
        //set weights
    }

    template <typename... Params, Number T>
    void AddLayer(size_t loc, T sz, Params... p)
    {
        layers[loc] = Mat<double>(sz, layers[loc-1].sizeX()); //weight
        layers[loc+1] = Mat<double>(sz, 1); //biases
        loc+=2;
        if constexpr(sizeof...(p)) AddLayer(loc, p...);
    }
    template <int sampleSize = 1>
    Mat<double> ForwardProp() const //returns iteration cost
    {
        if constexpr(!sampleSize) throw std::invalid_argument("sample size must be higher then 0");
        if constexpr(sampleSize > 1) // multi threading condition
        {
            //make {sampleSize} amount of threads
        }
        else return ForwardProp();
    }
    //add back prop

    ~NeuralNetwork()
    {
        delete[] layers;
    }
};
