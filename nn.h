#pragma once
#include <random>
#include <memory>
#include <thread>
#include <cmath>
#include "mat.h"

class NeuralNetwork
{
private:
    Mat<double>* layers = nullptr;
    double learningRate = 0;
    size_t size = 0;
    double (*cost)(double,double);
    double (*cost_p)(double,double);
    double (*activation)(double);
    double (*activation_p)(double);

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
    NeuralNetwork() = delete;
    template <typename... Params>
    NeuralNetwork(double lr, double (*cost)(double,double), double (*cost_p)(double,double), double (*act)(double), double (*act_p)(double), size_t first, Params ...p) : learningRate((double)lr), cost(cost), cost_p(cost_p), activation(act), activation_p(act_p)
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
    }

    Mat<double> ForwardProp(const Mat<double>& input) const //returns output for given input
    {
        if( (input.sizeX() != layers[0].sizeX()) || (input.sizeY() != layers[0].sizeY()) ) throw std::invalid_argument("inputs must be the same dimensions as first layer");
        //loading in network state
        std::unique_ptr< Mat<double>[] > currentMat = std::make_unique< Mat<double>[]>(size); //smart pointers gang
        for(size_t i = 0; i < size; ++i)
        {
            currentMat[i] = Mat<double>(layers[i].sizeX(), layers[i].sizeY());
            for(size_t j = 0; j < layers[i].sizeY(); ++j)
            for(size_t k = 0; k < layers[i].sizeX(); ++k)
                currentMat[i].at(j, k) = layers[i].at(j, k);
        }

        //forward propogating
        currentMat[0] = input;
        for(size_t i = 0; i < size-1; i+=3)
        {
            currentMat[i+3] = ((currentMat[i].dot(currentMat[i+1]))+currentMat[i+2]); //H = (I dot W1)+B
            currentMat[i+3].ApplyFunction(activation);
        }
        
        return currentMat[size-1];
    }

    size_t Size() const { return size; }
    Mat<double> Cost(const Mat<double>& result, const Mat<double>& desired) const //gets cost of prediction
    {
        return result.op(desired, cost);
    }
    
    //add back prop

    ~NeuralNetwork()
    { 
        delete[] layers; 
    }
};

//add multi-threading forward prop for sampling
