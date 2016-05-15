#pragma once

#include "Level.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <unordered_map>
#include <string>

class Tane {
public:
	int attrNum, cnt;
	int* rhs;
	std::vector< std::vector<int> >* piTmp;
	bool* piCntTmp;
	int* piSumTmp;
	std::unordered_map<std::string, int> hashMap;
	std::vector<std::string*> table;

public:
	Tane() {}
	Tane(std::string dataFileName);

	void run(std::ostream& outputStream);
	int getCount();

public:
	void output(int x, int y, std::ostream& outputStream);
	int pi(int num);
	int piProduct(int a, int b);
	void computeDependencies(Level l, std::ostream& outputStream);
};