#pragma once
#include <string>
#include <bitset>
#include <exception>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"


using namespace cv;

class image
{
public:
	static Mat ImageEncode(std::string contents)
	{
		int sz = contents.size();
		Mat output = Mat::zeros(sz, 8, CV_8UC1);
		std::bitset<8> b;
		for (int i = 0; i < sz; ++i)
		{
			b = contents.at(i);
			for (int j = 0; j < 8; ++j)
			{
				output.at<uchar>(i, j) = b[j] * 255;
			}
		}
		return output;
	}
	//255 white
	//0 black
	static std::string ImageDecoder(const Mat& m)
	{
		if (m.cols != 8) throw new std::exception("mat has to be a width of 8");
		std::bitset<8> b;
		std::string res;
		for (int i = 0; i < m.rows; ++i)
		{
			for (int j = 0; j < 8; ++j)
			{
				b[j] = (m.at<uchar>(i, j) == 255);
			}
			res += b.to_ulong();
		}
		return res;
	}
};