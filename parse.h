#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

class TextParser sealed {
private:
	static vector<string>ParseLine(string line, char sep) {
		stringstream ss(line);
		vector<string> result;
		string temp;
		while (getline(ss, temp, sep)) {
			result.push_back(temp);
		}
		return result;
	}

public:
	static vector< vector<string> >Parse(string location, char sep, unsigned long numOfVars = NULL) {
		ifstream inputFile(location);
		vector<vector<string> >result;
		string line;
		for (int i = 0; getline(inputFile, line); ++i) {
			if (numOfVars != NULL) {
				if (ParseLine(line, sep).size() != numOfVars) throw new exception((i + 1) + "line has an invalid amount of parameters");
			}
			result.push_back(ParseLine(line, sep));
		}
		inputFile.close();
		return result;
	}
	static void Save(string location, vector<vector<string> > vec, char sep) {
		ofstream outputFile(location);
		for (int i = 0; i < vec.size(); ++i) {
			for (int j = 0; j < vec[i].size(); ++j) {
				outputFile << vec[i][j];
				if (j != vec[i].size() - 1) outputFile << sep;
			}
			outputFile << '\n';
		}
		
	}
};