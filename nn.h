#pragma once
#include <iostream> //to  be removed
#include <random>
#include <memory>
#include <thread>
#include "mat.h"

//use std::uniform_real_distribution<>

bool IsDone(std::shared_ptr<bool[]> bools, size_t range)
{
    for(size_t i = 0; i < range; ++i)
        if(!bools[i]) return false;

    return true;
}

class NeuralNetwork
{
private:
    Mat<double>* layers = nullptr;
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
    void ForwardProp(const Mat<double>& input, std::shared_ptr<Mat<double> > returnVal, std::shared_ptr<bool> done, size_t i) const //for each thread to run async
    {
        std::cout<<"+Thread "<<i<<" has started";
        std::unique_ptr< Mat<double>[] > currentMat = MakeCopy();
        //do stuff
        *returnVal = currentMat[size-1];
        *done = true;
        std::cout<<"-Thread "<<i<<" has ended";
    }
public: 
    template <typename... Params>
    NeuralNetwork(size_t first, Params ...p)
    {
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
        for(size_t i = 0; i < size; ++i)
            std::cout<<layers[i]<<'\n';
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
    Mat<double> ForwardProp(const Mat<double>& input) const //returns prediction
    {
        std::unique_ptr< Mat<double>[] > currentMat = MakeCopy();
        //do stuff
        return currentMat[size-1];
    }
    template <size_t sampleSize = 1>
    std::shared_ptr< Mat<double>[] > ForwardProp(std::unique_ptr< Mat<double>[] > inputs) //returns sample size of predicted outputs
    {
        if constexpr(sampleSize<=1) throw std::invalid_argument("sample size must be higher then 1");
        
        //make {sampleSize} amount of threads
        std::unique_ptr< std::thread[] > threads = std::make_unique< std::thread[] >(sampleSize);
        std::shared_ptr< Mat<double>[] > results = std::shared_ptr< Mat<double>[] >(sampleSize);
        std::shared_ptr< bool[] > dones = std::shared_ptr<bool[]>(sampleSize);
        for(size_t i = 0; i < sampleSize; ++i)
            threads[i] = std::thread(ForwardProp, inputs[i], results[i], dones[i], i);

        //continually check if all threads are done
        while(!IsDone(dones, sampleSize) ) { }
        for(size_t i = 0; i < sampleSize; ++i)
            threads[i].join();

        return results;
    }
    /*
    static Mat<double> Cost() //gets cost of prediction
    {

    }
    */
    //add back prop

    ~NeuralNetwork()
    {
        delete[] layers;
    }
};

//add activation func
