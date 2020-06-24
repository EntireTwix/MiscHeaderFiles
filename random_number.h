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
	unsigned lowest = 0, highest = 1, offset = 0;
public:
	Random() = default;
	Random(unsigned a, unsigned b)
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
	} 

	unsigned get() const 
	{
		return (double)((rand()%(int)(highest))+offset);	
	}
};
