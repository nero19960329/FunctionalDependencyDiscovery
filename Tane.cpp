#include "Tane.h"
#include "Utils.h"

#include <set>

using namespace std;

Tane::Tane(string dataFileName) {
	table = readData(dataFileName, attrNum);
	cnt = 0;
	piTmp = new int[1 << attrNum];
	memset(piTmp, 0, (1 << attrNum) * sizeof(int));
}

int Tane::getCount() {
	return cnt;
}

void Tane::output(int x, int y, ostream& outputStream) {
	int tmp;

	tmp = 1;
	for (int i = 1; i <= attrNum; ++i) {
		if (x & tmp) {
			outputStream << i << " ";
		}
		tmp <<= 1;
	}

	outputStream << "-> ";
	tmp = 1;
	for (int i = 1; i <= attrNum; ++i) {
		if (y & tmp) {
			outputStream << i << " ";
		}
		tmp <<= 1;
	}
	outputStream << endl;
}

int Tane::pi(int num) {
	if (piTmp[num] == 0) {
		vector<int> countNum;
		int tmpNum, res;
		string str;

		tmpNum = num;
		for (int i = 0; i < attrNum; ++i) {
			if (tmpNum % 2) {
				countNum.push_back(i);
			}
			tmpNum /= 2;
		}

		hashMap.clear();
		res = table.size();
		for (int i = 0; i < table.size(); ++i) {
			str = table[i][countNum[0]];
			if (countNum.size() > 1) {
				for (int j = 1; j < countNum.size(); ++j) {
					str += ",";
					str += table[i][countNum[j]];
				}
			}
			
			if (hashMap[str]) {
				--res;
			} else {
				hashMap[str] = true;
			}
		}
		piTmp[num] = res;
	}
	return piTmp[num];
}

void Tane::computeDependencies(Level l, ostream& outputStream) {
	set<int>::iterator it, begin, end;
	int x, tmpA, flag, shiftTmp, res, e, m, comple, tmpB;

	begin = l.elems.begin();
	end = l.elems.end();

	it = begin;
	while (it != end) {
		// for each X in L
		x = *it;
		tmpA = 1;
		
		flag = 0;
		shiftTmp = (1 << attrNum);
		res = shiftTmp - 1;

		// RHS+(X) := InterRHS+(X\{E})
		for (int i = 0; i < attrNum; ++i) {
			if (x & tmpA) {
				e = x - tmpA;
				res = res & rhs[e];
			}
			tmpA <<= 1;
		}
		rhs[x] = res;

		// foreach E belongs to X intersect RHS+(X) do
		tmpA = 1;
		m = x & rhs[x];
		for (int i = 0; i < attrNum; ++i) {
			if (m & tmpA) {
				// if X\{E} -> E is valid
				if (pi(x - tmpA) == pi(x)) {
					output(x - tmpA, tmpA, outputStream);
					++cnt;
					// remove E for RHS+(X)
					rhs[x] -= tmpA;
					// remove all F belongs to R\X from RHS+(X)
					comple = (shiftTmp - 1) - x;
					tmpB = 1;
					for (int j = 0; j < attrNum; ++j) {
						if (comple & tmpB) {
							if (rhs[x] & tmpB) {
								rhs[x] -= tmpB;
							}
						}
						tmpB <<= 1;
					}
				}
			}
			tmpA <<= 1;
		}

		// if RHS+(X) == empty do
		if (rhs[x] == 0) {
			l.elems.erase(it++);
		} else {
			++it;
		}
	}
}

void Tane::run(ostream& outputStream) {
	vector<Level> levels;

	// initialize rhs
	int shiftTmp = (1 << attrNum);
	rhs = new int[shiftTmp];
	int max = shiftTmp;
	memset(rhs, 0, max * sizeof(int));
	int tmp = 1;
	for (int i = 0; i < attrNum; ++i) {
		rhs[tmp] = max - 1;
		tmp <<= 1;
	}

	//main loop
	for (int i = 1; i <= attrNum; ++i) {
		Level lev(i, attrNum);
		if (i > 1) {
			lev.initialize(levels[i - 2]);
		}
		levels.push_back(lev);

		if (i > 1) {
			computeDependencies(lev, outputStream);
		}
	}
}