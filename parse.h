#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include <exception>

class TextParser final
{
private:
	static std::vector<std::string> ParseLine(const std::string &line, char sep)
	{
		std::stringstream ss(line);
		std::vector<std::string> result;
		std::string temp;
		while (getline(ss, temp, sep))
		{
			result.push_back(temp);
		}
		return result;
	}

public:
	static std::vector<std::vector<std::string>> Parse(const std::string_view &location, char sep, unsigned long numOfVars)
	{
		std::ifstream inputFile(location);
		std::vector<std::vector<std::string>> result;
		std::string line;
		while (getline(inputFile, line))
		{
			if (numOfVars != 0)
			{
				if (ParseLine(line, sep).size() != numOfVars)
					throw new std::invalid_argument((i + 1) + "line has an invalid amount of parameters");
			}
			result.push_back(ParseLine(line, sep));
		}
		inputFile.close();
		return result;
	}
	static std::vector<std::vector<std::string>> Parse(const std::string_view &location, char sep)
	{
		std::ifstream inputFile(location);
		std::vector<std::vector<std::string>> result;
		std::string line;
		while (getline(inputFile, line))
			result.push_back(ParseLine(line, sep));
		inputFile.close();
		return result;
	}
	static std::vector<std::string> Parse(const std::string_view &location)
	{
		std::ifstream inputFile(location);
		std::vector<std::string> result;
		std::string line;
		while (getline(inputFile, line, '\n'))
			result.push_back(line);
		inputFile.close();
		return result;
	}

	static void Save(const std::string_view &location, const std::vector<std::vector<std::string>> &vec, char sep, unsigned short int thresholdSize)
	{
		std::ofstream outputFile(location);
		for (int i = 0; i < vec.size(); ++i)
		{
			for (int j = 0; j < vec[i].size(); ++j)
			{
				outputFile << vec[i][j];
				if (j != vec[i].size() - 1)
					outputFile << sep;
			}
			if (vec[i].size() >= thresholdSize)
				outputFile << '\n';
		}
		outputFile.close();
	}
	template <typename T>
	static void Save(const std::string_view &location, const std::vector<T> &vec, char sep)
	{
		std::ofstream outputFile(location);
		for (int i = 0; i < vec.size(); ++i)
		{
			outputFile << vec[i] << sep;
		}
		outputFile.close();
	}

	template <typename T>
	static void Save(const std::string_view &location, const std::vector<T> &vec)
	{
		std::ofstream outputFile(location);
		for (int i = 0; i < vec.size(); ++i)
		{
			outputFile << vec[i];
		}
		outputFile.close();
	}
};
