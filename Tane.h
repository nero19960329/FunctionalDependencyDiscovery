#pragma once

#include "Level.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>

class Tane {
private:
	int attrNum, cnt;
	int* rhs;
	int* piTmp;
	std::unordered_map<std::string, bool> hashMap;
	std::vector<std::string*> table;

public:
	Tane(std::string dataFileName);

	void run(std::ostream& outputStream);
	int getCount();

private:
	void output(int x, int y, std::ostream& outputStream);
	int pi(int num);
	void computeDependencies(Level l, std::ostream& outputStream);
};