#include "Tane.h"
#include "Utils.h"

#include <set>

using namespace std;

Tane::Tane(string dataFileName) {
	int tmp = (1 << attrNum);

	table = readData(dataFileName, attrNum);
	cnt = 0;
	//piTmp = new int[1 << attrNum];
	piTmp = new vector< vector<int> >[tmp];
	piCntTmp = new bool[tmp];
	memset(piTmp, 0, tmp * sizeof(int));
	memset(piCntTmp, 0, tmp * sizeof(bool));
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
	if (piCntTmp[num]) {
		return piTmp[num].size();
	}

	vector<int> countNum;
	int tmpNum;
	string str;
	int tmpIndex;
	vector<int> tmpVec;
	vector< vector<int> > tmpSet;
	vector< vector<int> >::const_iterator vecIt;
	unordered_map<string, int>::const_iterator hashIt;

	tmpNum = num;
	for (int i = 0; i < attrNum; ++i) {
		if (tmpNum % 2) {
			countNum.push_back(i);
		}
		tmpNum /= 2;
	}

	if (countNum.size() == 1) {
		// first, use unordered_map to do the partition
		tmpIndex = 0;
		hashMap.clear();
		for (int i = 0; i < table.size(); ++i) {
			str = table[i][countNum[0]];

			hashIt = hashMap.find(str);
			if (hashIt != hashMap.end()) {
				tmpSet[hashIt->second].push_back(i);
			} else {
				hashMap.insert(make_pair(str, tmpIndex++));
				tmpVec = vector<int>();
				tmpVec.push_back(i);
				tmpSet.push_back(tmpVec);
			}
		}

		// then, drop the set that length equals to 1
		for (vecIt = tmpSet.cbegin(); vecIt != tmpSet.cend(); ++vecIt) {
			if (vecIt->size() != 1) {
				piTmp[num].push_back(*vecIt);
			}
		}

		piCntTmp[num] = true;
		return piTmp[num].size();
	} else {
		cout << "error from pi!\n" << endl;
		throw -1;
	}
}

int Tane::piProduct(int a, int b) {
	if (piCntTmp[a + b]) {
		return piTmp[a + b].size();
	}

	vector<int> countNum;
	int tmpNum;
	int lenA, lenB;
	int *T;
	vector< vector<int> > S;
	vector<int> tmpVec;

	tmpNum = a + b;
	for (int i = 0; i < attrNum; ++i) {
		if (tmpNum % 2) {
			countNum.push_back(i);
		}
		tmpNum /= 2;
	}

	if (countNum.size() == 1) {
		throw -1;
	} else {
		lenA = pi(a);
		lenB = pi(b);
		T = new int[table.size()];
		S = vector< vector<int> >(lenA);
		tmpVec = vector<int>();

		memset(T, -1, table.size() * sizeof(int));
		for (int i = 0; i < lenA; ++i) {
			for (auto t : piTmp[a][i]) {
				T[t] = i;
			}
			S.push_back(tmpVec);
		}

		for (int i = 0; i < lenB; ++i) {
			for (auto t : piTmp[b][i]) {
				if (T[t] >= 0) {
					S[T[t]].push_back(t);
				}
			}
			for (auto t : piTmp[b][i]) {
				if (T[t] >= 0) {
					if (S[T[t]].size() >= 2) {
						piTmp[a + b].push_back(S[T[t]]);
					}
					S[T[t]] = tmpVec;
				}
			}
		}

		piCntTmp[a + b] = true;
		return piTmp[a + b].size();
	}
}

int Tane::error(int num) {
	int a, b;

	b = pi(num);
	a = 0;

	for (int i = 0; i < piTmp[num].size(); ++i) {
		a += piTmp[num][i].size();
	}
	//for (auto t : piTmp[num]) {
	//	a += t.size();
	//}

	return a - b;
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
				pi(x - tmpA);
				piProduct(x - tmpA, tmpA);
				if (error(x - tmpA) == error(x)) {
					//output(x - tmpA, tmpA, outputStream);
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