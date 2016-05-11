#include "Level.h"

#include <iostream>
#include <map>

using namespace std;

Level::Level(int index, int attributeNum) {
	levelIndex = index;
	attrNum = attributeNum;
	if (index == 1) {
		int tmp = 1;
		for (int i = 0; i < attrNum; ++i) {
			elems.insert(tmp);
			tmp <<= 1;
		}
	}
}

Level::~Level() {
	elems.clear();
}

void Level::initialize(Level preLevel) {
	set<int>::iterator it, begin, end;
	begin = preLevel.elems.begin();
	end = preLevel.elems.end();
	for (it = begin; it != end; ++it) {
		int num = *it;
		int tmp = 1;
		for (int i = 0; i < attrNum; ++i) {
			if (!(num & tmp)) {
				elems.insert(num + tmp);
			}
			tmp <<= 1;
		}
	}
}