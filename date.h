#pragma once
#include <iostream>
#include <ctime>
#include <string>
#include <sstream>
#include <climits>

using namespace std;

namespace date {
	unsigned char MonthLength(unsigned char month, unsigned short int year) 
	{
		if (month > 12) throw new exception("month cant be bigger then 12");
		if (month <= 0) throw new exception("month cant be a negative value");
		if (year < 0) throw new exception("year cant be a negative value");
		switch (month) {
		case 1:
			return 31;
		case 2:
			if (year % 4 == 0) return 29;
			return 28;
		case 3:
			return 31;
		case 4:
			return 30;
		case 5:
			return 31;
		case 6:
			return 30;
		case 7:
			return 31;
		case 8:
			return 31;
		case 9:
			return 30;
		case 10:
			return 31;
		case 11:
			return 30;
		case 12:
			return 31;
		default:
			throw new exception("unkown error occured");
		}
	}

	class Date sealed {
	private:
		unsigned char _day = 0, _month = 1;
		unsigned short int _year = 0;
	public:
		Date() = default;
		Date(unsigned char d, unsigned char m, unsigned short int y = 0) : _day(d), _month(m), _year(y)															//default
		{
			if (((d < 0) || (m < 1) || (y < 0)) || ((d > date::MonthLength(m, y)) || (m > 12))) throw new exception("invalid parameters");
		}
		explicit Date(unsigned int a)																															//constructor with number of days
		{
			if (a < 0) throw new exception("date can not be negative");
			for (int i = 0; i < a; ++i) {
				//if days valid
				if (_day < MonthLength(_month, _year))
				{
					//add day
					_day++;
				}
				else
				{
					//if months valid
					if (_month < 12)
					{
						//reset day
						_day = 1;
						//add month
						_month++;
					}
					else
					{
						//reset days
						_day = 1;
						//reset months
						_month = 1;
						//add year
						if (_year == USHRT_MAX) throw new exception("Date has exceeded max size");
						_year++;
					}
				}
			}
		}
		Date& operator=(unsigned int a)																															//constructor with number of days
		{
			if (a < 0) throw new exception("date can not be negative");
			_day = 0;
			_month = 1;
			_year = 0;
			for (int i = 0; i < a; ++i) {
				//if days valid
				if (_day < MonthLength(_month, _year))
				{
					//add day
					_day++;
				}
				else
				{
					//if months valid
					if (_month < 12)
					{
						//reset day
						_day = 1;
						//add month
						_month++;
					}
					else
					{
						//reset days
						_day = 1;
						//reset months
						_month = 1;
						//add year
						if (_year == USHRT_MAX) throw new exception("Date has exceeded max size");
						_year++;
					}
				}
			}
			return *this;
		}
		operator unsigned int() const noexcept																														//days total conversion
		{
			unsigned int sum = 0;
			unsigned char d = _day;
			unsigned char m = _month;
			unsigned short int y = _year + 1;
			while (!((d > 1) && (m > 1) && (y == 0))) {
				if (d > 1)
				{
					//decrement
					d--;
				}
				else
				{
					//if month is valid
					if (m > 1)
					{
						//decrement month
						m--;
						//reset days
						d = MonthLength(m, y);
					}
					else
					{
						//decrement year
						y--;

						//reset month and month
						m = 12;
						d = MonthLength(m, y);
					}
				}
				sum++;
			}
			return sum;
		}
		static Date CurrentDate() noexcept																															//gets current date function 
		{
			Date res(((time(NULL) + 57600) / 86400) + 719543); //hopefully fixed
			return res;
		}
		friend ostream& operator<<(ostream& output, const Date& d)																									//prints date	
		{
			return output << d.month() << '/' << d.day() << '/' << d.year() << '\n';
		}
		friend istream& operator>>(istream& input, Date& d)																											//parses input into date
		{
			string params[3];
			string line;
			input >> line;
			stringstream ss(line);
			int size = 0;
			for (int i = 0; getline(ss, line, '/'); ++i) {
				if (i == 4) throw new exception("invalid number of parameters for class:Date");
				params[i] = line;
				size = i;
			}
			//size check
			if (size != 2) throw new exception("invalid number of parameters for class:Date");

			//parameter checks
			try {
				d = Date(stoi(params[1]), stoi(params[0]), stoi(params[2]));
			}
			catch (...) {
				throw new exception("invalid parameters for Date");
			}

			return input;
		}
		bool operator ==(const Date& b) const																														//operator ==
		{
			return ((day() == b.day()) && (month() == b.month()) && (year() == b.year()));
		}
		bool operator !=(const Date& b) const																														//operator !=
		{
			return ((day() != b.day()) || (month() != b.month()) || (year() != b.year()));
		}

																																									//read-only functions
		unsigned short int day() const { return _day; }
		unsigned short int month() const { return _month; }
		unsigned short int year() const { return _year; }
	};

}
