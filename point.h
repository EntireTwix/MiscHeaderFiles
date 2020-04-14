#pragma once
#include <ostream>
#include <vector>

template <int sz>
class Point
{
private:
	float* members = nullptr;
public:
	Point() noexcept { if (size() > 0) members = new float[size()]{ 0 }; }
	Point(std::initializer_list<float> list)
	{
		if (list.size() > size()) throw std::invalid_argument("list size can not be greater then size of Point");
		float* p = new float[size()]{ 0 };
		{
			int i = 0;
			for (float d : list)
			{
				p[i] = d;
				++i;
			}
		}
		if (size() > 0) members = std::move(p);
	}
	explicit Point(const std::vector<float>& list)
	{
		if (list.size() > size()) std::invalid_argument("list size can not be greater then size of Point");
		if (size() > 0) members = new float[size()]{ 0 };
		for (int i = 0; i < list.size(); ++i)
		{
			members[i] = list[i];
		}
	}
	explicit Point(Point&& p) noexcept { members = std::move(p.members); sz = p.sz; p.sz = 0; }
	void operator=(Point&& p) noexcept { members = std::move(p.members); sz = p.sz; p.sz = 0; }
	void operator=(Point& p) noexcept { members = std::copy(p.members); sz = p.sz; }
	~Point() { delete[] members; }
	
	float& operator[](int pos)
	{
		if ((pos < 0) || (pos > size())) throw std::out_of_range("cant be negative or greater then size()");

		return members[pos];
	}

	constexpr unsigned size() const { if (sz < 0) return NULL; return sz; }
	const unsigned size_of = size() * 4;

	template <int I>
	friend std::ostream& operator<<(std::ostream& os, const Point<I>& p) noexcept
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

using Point2 = Point<2>;
using Point3 = Point<3>;
