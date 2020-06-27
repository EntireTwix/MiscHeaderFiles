#pragma once
#include <iostream> //to  be removed
#include <random>
#include <memory>
#include <thread>
#include "mat.h"

class NeuralNetwork
{
private:
    Mat<double>* layers = nullptr;
    double learningRate = 0;
    size_t size = 0; //possibly irrelevant variable

    std::unique_ptr< Mat<double>[] > MakeCopy() const
    {
        std::unique_ptr< Mat<double>[] > net = std::make_unique< Mat<double>[]>(size); //smart pointers gang
        for(size_t i = 0; i < size; ++i)
        {
            net[i] = Mat<double>(layers[i].sizeX(), layers[i].sizeY());
            for(size_t j = 0; j < layers[i].sizeY(); ++j)
            for(size_t k = 0; k < layers[i].sizeX(); ++k)
                net[i].at(j, k) = layers[i].at(j, k);
        }
        return net;
    }
    template <typename... Params, Number T>
    void AddLayer(size_t loc, T sz, Params... p)
    {
        layers[loc] = Mat<double>(sz, layers[loc-1].sizeX()); //weight
        layers[loc+1] = Mat<double>(sz, 1); //biases
        layers[loc+2] = Mat<double>(sz, 1); //activations
        loc+=3;
        if constexpr(sizeof...(p)) AddLayer(loc, p...);
    }
public: 
    template <typename... Params>
    NeuralNetwork(double lr, size_t first, Params ...p) : learningRate((double)lr)
    {
        if(lr<=0) throw std::invalid_argument("learning rate must be positive");  
        //making layers
        size = ((sizeof...(p))*3)+1;
        layers = new Mat<double>[size];
        layers[0] = Mat<double>(first, 1); 
        AddLayer(1, p...);

        //initilizing weights
        std::random_device rd; 
        std::mt19937 gen(rd()); 
        for(size_t i = 1; i < size; i+=3)
        {
            std::uniform_real_distribution<> dis(-(1/std::sqrt(layers[i].sizeX())), (1/std::sqrt(layers[i].sizeX())));
            for(size_t j = 0; j < layers[i].sizeY(); ++j)
            for(size_t k = 0; k < layers[i].sizeX(); ++k)
                layers[i].at(j, k) = dis(gen);
        }

        //for debugging
        std::cout<<"Network being constructed\n";
        for(size_t i = 0; i < size; ++i)
            std::cout<<layers[i]<<'\n';
    }

    Mat<double> ForwardProp(const Mat<double>& input) const //returns output for given input
    {
        if( (input.sizeX() != layers[0].sizeX()) || (input.sizeY() != layers[0].sizeY()) ) throw std::invalid_argument("inputs must be the same dimensions as first layer");
        std::unique_ptr< Mat<double>[] > currentMat = MakeCopy();

        return currentMat[size-1];
    }
    /*
    static Mat<double> Cost(const Mat<double>& result, const Mat<double>& desired) //gets cost of prediction
    {
        if( (result.sizeX() != desired.sizeX()) || (result.sizeY() != desired.sizeY()) ) throw std::invalid_argument("result and desired matrices must have the same dimensions");

    }
    */
    
    //add back prop

    ~NeuralNetwork(){ delete[] layers; }
};
