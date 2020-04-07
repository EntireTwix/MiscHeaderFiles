#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include <exception>

class TextParser sealed {
private:
	static std::vector<std::string>ParseLine(std::string line, char sep) {
		std::stringstream ss(line);
		std::vector<std::string> result;
		std::string temp;
		while (getline(ss, temp, sep)) {
			result.push_back(temp);
		}
		return result;
	}
	
public:
	static std::vector<std::string>Parse(std::string location)
	{
		std::ifstream inputFile(location);
		std::vector<std::string>result;
		std::string line;
		for (int i = 0; getline(inputFile, line, '\n'); ++i) {
			result.push_back(line);
		}
		inputFile.close();
		return result;
	}

	static std::vector< std::vector<std::string> >Parse(std::string location, char sep, unsigned long numOfVars = NULL) {
		std::ifstream inputFile(location);
		std::vector<std::vector<std::string> >result;
		std::string line;
		for (int i = 0; getline(inputFile, line); ++i) {
			if (numOfVars != NULL) {
				if (ParseLine(line, sep).size() != numOfVars) throw new std::exception((i + 1) + "line has an invalid amount of parameters");
			}
			result.push_back(ParseLine(line, sep));
		}
		inputFile.close();
		return result;
	}

	static void Save(std::string location, std::vector<std::vector<std::string>> vec, char sep, unsigned short int thresholdSize = 0) {
		std::ofstream outputFile(location);
		for (int i = 0; i < vec.size(); ++i) {
			for (int j = 0; j < vec[i].size(); ++j) {
				outputFile << vec[i][j];
				if (j != vec[i].size() - 1) outputFile << sep;
			}
			if (vec[i].size() >= thresholdSize) outputFile << '\n';
		}
		outputFile.close();
	}

	template<typename T>
	static void Save(std::string location, std::vector<T> vec, char sep) {
		std::ofstream outputFile(location);
		for (int i = 0; i < vec.size(); ++i) {
			outputFile << vec[i] << sep;
		}
		outputFile.close();
	}
};
