#pragma once
#include <random>
#include <ctime>
#include <stdexcept>

constexpr unsigned char GetPrecision(double x)
{
	unsigned char i = 0;
	while(x > (int)x)
	{
		x*=10;
		++i;
	}
	return i;
}

class Random final
{
private:
	double lowest = 0, highest = 10, offset = 0;
	unsigned precision = 10;
public:
	Random() = default;
	Random(double a, double b, unsigned p = 1)
	{
		if( (a < 0) || (b < 0) ) throw std::invalid_argument("numbers must be positive");
		//setting highest to the higher of the two, and finding offset
		if(a == b) throw std::out_of_range("first and second argument must be different");
		else if(a > b) 
		{
			highest = a;
			lowest = b;
		}
		else if(b > a)
		{
			highest = b;
			lowest = a;
		}

		//setting offset
		if(lowest != 0)
		{
			offset = lowest;
			lowest = 0;
			highest -= lowest;
		}
		
		precision = std::pow(10, p);

		//turning double into ints basically
		highest*=precision;
		lowest*=precision;
		offset*=precision;
	} 

	double get() const 
	{
		return (double)((rand()%(int)(highest))+offset)/precision;	
	}
};
