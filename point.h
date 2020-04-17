#pragma once
#include <ostream>

template <int sz = 0, typename Num = float >
class Point
{
private:
	Num* members = new Num[size() == 0 ? 1 : size()]{ 0 };
public:
	Point() = default;
	Point(std::initializer_list<Num> list)
	{
		if (list.size() == 0) throw std::invalid_argument("list must be atleast 1 item");
		Num* params = new Num[list.size()];
		int j = 0;
		for (Num n : list)
		{
			params[j] = n;
			++j;
		}

		if (size() < list.size()) throw std::invalid_argument("list of arguments can't be larger then initilized size");
		for (unsigned i = 0; i < list.size(); ++i)
		{
			this->members[i] = params[i];
		}
		delete[] params;

	}
	Point(const Point<sz, Num>& p) noexcept
	{
		for (unsigned i = 0; i < p.size(); ++i)
		{
			members[i] = p[i];
		}
	}
	Point<sz, Num> operator=(const Point<sz, Num>& p) noexcept
	{
		for (unsigned i = 0; i < p.size(); ++i)
		{
			members[i] = p[i];
		}
		return *this;
	}
	Point(Point<sz, Num>&& p) noexcept
	{
		members = p.members;
		p.members = nullptr;
	}
	Point<sz, Num> operator=(Point<sz, Num>&& p) noexcept
	{
		members = p.members;
		p.members = nullptr;
		return *this;
	}
	~Point() { delete[] members; }

	constexpr unsigned size() const { return (sz > 0) ? sz : 0; }
	friend std::ostream& operator<<(std::ostream& os, const Point<sz, Num>& p)
	{
		os << "{ ";
		for (int i = 0; i < p.size(); ++i)
		{
			os << p[i] << ((i == p.size() - 1) ? " }" : ", ");
		}
		return os;
	}
	Num& operator[](int pos)
	{
		if (pos >= size() || pos < 0) throw new std::out_of_range("can't index out of size");
		return members[pos];
	}
	Num operator[](int pos) const
	{
		if (pos >= size() || pos < 0) throw new std::out_of_range("can't index out of size");
		return members[pos];
	}
};

template <int sz>
using PointD = Point<sz, double>;	
template <int sz>
using PointLD = Point<sz, long double>;

using Point3 = Point<3>;
using Point2 = Point<2>;
