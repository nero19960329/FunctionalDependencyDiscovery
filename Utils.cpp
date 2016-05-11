#include "Utils.h"

#include <fstream>

using namespace std;

int getCharCount(string str, char ch) {
	int len = str.length();
	int count = 0;

	for (int i = 0; i < len; ++i) {
		if (str.at(i) == ch) {
			++count;
		}
	}

	return count;
}

vector<string*> readData(string fileName, int& col) {
	ifstream fin(fileName);
	string tmp = "";
	vector<string*> res;

	// get column number
	if (!fin.eof()) {
		getline(fin, tmp, '\n');
		col = getCharCount(tmp, ',') + 1;
	} else {
		throw - 1;
	}

	// set file pointer to head
	fin.clear();
	fin.seekg(0);

	// read data
	while (true) {
		string* rowStr = new string[col];
		for (int j = 0; j < col - 1; ++j) {
			getline(fin, rowStr[j], ',');
		}
		getline(fin, rowStr[col - 1], '\n');

		if (rowStr[0] == "") {
			break;
		}
		res.insert(res.end(), rowStr);
	}

	return res;
}