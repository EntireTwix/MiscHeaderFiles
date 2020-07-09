#pragma once
#include <random>
#include "mat.h"

class Layer //abstract for NN
{
public:
    virtual Mat ForwardProp(const Mat& input) const = 0; 
    virtual Mat BackwardProp(const Mat& cost) = 0;
};

class Weight : public Layer
{
private:
    Mat weights;
public:    
    Weight(size_t x, size_t y, double min, double max) //x is next, y is previous 
    {
        weights = Mat(x, y);

        //setting initial weights
        std::default_random_engine generator;
        std::uniform_real_distribution<double> distribution(min,max);
        for(size_t i = 0; i < x; ++i)
        for(size_t j = 0; j < y; ++j)
            weights.At(i, j) = distribution(generator);
    }
    
    virtual Mat ForwardProp(const Mat& input) const override
    {
        if(input.SizeY()!=1) throw std::invalid_argument("input must be a vector for weight layer");
        if(input.SizeX() != weights.SizeY()) throw std::invalid_argument("y of input must equal x of weight");
        return input.Dot(weights);
    }
    virtual Mat BackwardProp(const Mat& cost) override
    {
        //do stuff
        return cost;
    }
};