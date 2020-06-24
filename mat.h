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
			if( (vec.size() != last) && (i != 0) ) throw std::invalid_argument("width has to be a constant in the Matrix");
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
		if ((x < 1) || (y < 1)) throw std::invalid_argument("both x and y must be positive");

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
	Type& at(size_t x, size_t y)
	{
		if (x >= size_x || x < 0) throw std::out_of_range("x is out of range");
		if (y >= size_y || y < 0) throw std::out_of_range("y is out of range");
		return members[y][x];
	}
	Type at(size_t x, size_t y) const
	{
		if (x >= size_x || x < 0) throw std::out_of_range("x is out of range");
		if (y >= size_y || y < 0) throw std::out_of_range("y is out of range");
		return members[y][x];
	}
	size_t sizeX() const { return size_x; }
	size_t sizeY() const { return size_y; }

	friend std::ostream& operator<<(std::ostream& os, const Mat& m)
	{
		for (int i = 0; i < m.sizeY(); ++i)
		{
			for (int j = 0; j < m.sizeX(); ++j)
			{
				os<<m.at(j, i)<<' ';
			}
			os << '\n';
		}
		return os<<'\n';
	}

	~Mat() { delete[] members;  }
};
