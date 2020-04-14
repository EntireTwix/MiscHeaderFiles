#pragma once
#include <ostream>
#include <vector>

class Point
{
private:
	float* members = nullptr;
	int sz = 0;
public:
	Point(int sz) noexcept { this->sz = sz; if (size() > 0) members = new float[size()]{ 0 }; }
	Point(std::initializer_list<float> list)
	{
		float* p = new float[list.size()]{ 0 };
		{
			int i = 0;
			for (float d : list)
			{
				p[i] = d;
				++i;
			}
		}
		members = new float[list.size()];
		sz = list.size();
		if (list.size() > 0)
		{
			for (int i = 0; i < list.size(); ++i) members[i] = float(p[i]);
		}
		delete[] p;
	}
	Point& operator=(std::initializer_list<float> list) 
	{ 
		if (list.size() > size()) throw std::invalid_argument("list size can not be greater then size of Point");
		float* p = new float[list.size()]{ 0 };
		{
			int i = 0;
			for (float d : list)
			{
				p[i] = d;
				++i;
			}
		}
		if(members == nullptr) members = new float[size()]{ 0 };
		if (size() > 0)
		{
			for (int i = 0; i < list.size(); ++i) members[i] = float(p[i]);
		}
		delete[] p;
		return *this; 
	};
	Point(const Point& p) noexcept
	{
		members = new float[p.sz];
		for (int i = 0; i < p.sz; ++i)
		{
			members[i] = p.members[i];
		}
		sz = p.sz;
	}

	~Point() { delete[] members; }
	
	void ApplyFunction(void (*func)(float&)) { for (int i = 0; i < size(); ++i) func(members[i]); }
	constexpr unsigned size() const { if (sz < 0) return 1; return sz; }
	float& operator[](int pos)
	{
		if ((pos < 0) || (pos > size())) throw std::out_of_range("cant be negative or greater then size()");
		return members[pos];
	}

	friend std::ostream& operator<<(std::ostream& os, const Point& p) noexcept
	{
		os << "{ ";
		for (int i = 0; i < p.size(); ++i)
		{
			os << p.members[i];
			if (i != p.size() - 1) os << ", ";
		}
		os << " }\n";
		return os;
	}
};

//TO-DO: move constructors
