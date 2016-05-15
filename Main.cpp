#include "Tane.h"
#include <iostream>
#include <fstream>
#include <ctime>

#include <vector>

using namespace std;

void run() {
	ofstream fout("./result.txt");

	time_t begin, end;
	begin = clock();

	cout << "============================================" << endl;
	cout << "Initializing tane algorithm . . ." << endl;
	Tane tane = Tane("./big_data.txt");
	cout << "Initialize ok!" << endl;
	cout << "============================================" << endl;
	cout << "Running algorithm . . ." << endl;
	tane.run(fout);
	cout << "============================================" << endl;
	cout << "Number of functional dependencies : " << tane.getCount() << endl;
	cout << "Get more detail in result.txt" << endl;

	end = clock();
	cout << "Runtime : " << double(end - begin) / CLOCKS_PER_SEC << endl;
	cout << "============================================" << endl;

	fout.flush();
	fout.close();
}

int main() {
	run();

	return 0;
}