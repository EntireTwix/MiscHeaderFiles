#pragma once
#include <ostream>
#include <initializer_list>
#include "generics.h"

template <ArithmeticOperators Type>
class Mat
{
private:
	Type** members = nullptr;
	size_t size_x = 0, size_y = 0;
public:
	using value_type = Type;
	Mat() = default;
	Mat(std::initializer_list<std::initializer_list<Type> > list)
	{
		size_t last = 0, i = 0;
		for (auto vec : list)
		{
			if( (vec.size() != last) && (i != 0) ) throw std::invalid_argument("Mat(list{{list}}):width has to be a constant in the Matrix");
			last = vec.size();
			++i;
		}

		unsigned w = 0, h = 0;
		members = new Type * [list.size()];

		//y
		for (auto vec : list)
		{
			members[h] = new Type[vec.size()];

			//x
			for (auto instance : vec)
			{
				members[h][w] = instance;
				++w;
			}
			w = 0;
			++h;
		}

		size_x = last;
		size_y = h;
	}
	explicit Mat(int x, int y)
	{
		if ((x < 1) || (y < 1)) throw std::invalid_argument("Mat(x,y):both x and y must be positive");

		//make Matrix
		members = new Type * [y];
		for (int i = 0; i < y; ++i)
		{
			members[i] = new Type[x]{Type()};
		}

		//update to size
		size_x = x;
		size_y = y;
	}
	
	//move operators
	Mat(Mat<Type>&& m)
	{
		members = m.members;
		size_x = m.size_x;
		size_y = m.size_y;
		m.members = nullptr;
		m.size_y = m.size_x = 0;
	}
	Mat<Type> operator=(Mat<Type>&& m)
	{
		members = m.members;
		size_x = m.size_x;
		size_y = m.size_y;
		m.members = nullptr;
		m.size_y = m.size_x = 0;
		return *this;
	}

	//assignment operators
	Mat(const Mat<Type>& m)
	{
		if (m.members)
		{
			members = new Type * [m.size_y];
			
			for (int j = 0; j < m.size_y; ++j)
			{
				members[j] = new Type[m.size_x];
				for (int i = 0; i < m.size_x; ++i)
				{
					members[j][i] = m.members[j][i];
				}
			}
		}
		size_x = m.sizeX();
		size_y = m.sizeY();
	}
	Mat<Type> operator=(const Mat<Type>& m)
	{
		if (m.members)
		{
			members = new Type * [m.size_y];

			for (int j = 0; j < m.size_y; ++j)
			{
				members[j] = new Type[m.size_x];
				for (int i = 0; i < m.size_x; ++i)
				{
					members[j][i] = m.members[j][i];
				}
			}
		}
		size_x = m.sizeX();
		size_y = m.sizeY();
		return *this;
	}

	//at function
	Type& at(size_t y, size_t x)
	{
		if (x >= size_x || x < 0) throw std::out_of_range("at&:x is out of range "+std::to_string(x));
		if (y >= size_y || y < 0) throw std::out_of_range("at&:y is out of range "+std::to_string(y));
		return members[y][x];
	}
	Type at(size_t y, size_t x) const
	{
		if (x >= size_x || x < 0) throw std::out_of_range("at:x is out of range "+std::to_string(x));
		if (y >= size_y || y < 0) throw std::out_of_range("at:y is out of range "+std::to_string(y));
		return members[y][x];
	}	

	void ApplyFunction(void (*func)(Type&)) //for each
	{
		for(size_t i = 0; i < sizeY(); ++i)
		for(size_t j = 0; j < sizeX(); ++j)
			func(at(i, j));
	}

	template <typename... Params>
	void ApplyFunction(void (*func)(Type&, Params...), Params... p) //for each
	{
		for(size_t i = 0; i < sizeY(); ++i)
		for(size_t j = 0; j < sizeX(); ++j)
			func(at(i, j), p...);
	}
	
	size_t sizeX() const { return size_x; }
	size_t sizeY() const { return size_y; }

	Mat<Type> dot(const Mat<Type>& mat) const
	{
		Mat<Type> res(mat.sizeX(), sizeY()); //product dimensions are the x of both mats
		Type amount;
		for(size_t i = 0; i < sizeY(); ++i) //for each y on the A matrix 
		for(size_t j = 0; j < mat.sizeX(); ++j) //for each x on the B matrix 
		{
			amount = 0;
			for(int k = 0; k < sizeX(); ++k) //is equivelant to mat.sizeY() 
				amount+=at(i, k)*mat.at(k, j); 
			res.at(i, j) = amount;
		}
		return res;
	}
	Mat<Type> operator+(const Mat<Type>& mat) const
	{
		if( (mat.sizeX() > sizeX()) || (mat.sizeY() > sizeY()) ) throw std::invalid_argument("operator+:matrix being added must be less then or equal dimensions");
		Mat<Type> res(sizeX(), sizeY());

		for(size_t i = 0; i < mat.sizeY(); ++i)
		for(size_t j = 0; j < mat.sizeX(); ++j)
			res.at(i, j) = at(i, j)+mat.at(i, j);
		return res;
 	}
	Mat<Type> op(const Mat<Type>& mat, Type (*func)(Type a, Type b)) const //general operations func
	{
		if( (mat.sizeX() != sizeX()) || (mat.sizeY() != sizeY()) ) throw std::invalid_argument("op:matrix being added must be less then or equal dimensions");
		Mat<Type> res(sizeX(), sizeY());
		for(size_t i = 0; i < sizeY(); ++i)
		for(size_t j = 0; j < sizeX(); ++j)
		{
			res(i, j) = func(at(i, j), mat.at(i, j));
		}
		return res;
	}

	friend std::ostream& operator<<(std::ostream& os, const Mat& m)
	{
		for (size_t i = 0; i < m.sizeY(); ++i)
		{
			for (size_t j = 0; j < m.sizeX(); ++j)
			{
				os<<m.at(i, j)<<' ';
			}
			os << '\n';
		}
		return os;
	}

	~Mat() { delete[] members; }
};

//add find func
//add iterator
