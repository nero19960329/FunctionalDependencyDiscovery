#pragma once

#include <set>

class Level {
public:
	int levelIndex;
	int totalElems;
	int attrNum;
	std::set<int> elems;

public:
	Level(int index, int attributeNum);
	~Level();

	void initialize(Level preLevel);
};