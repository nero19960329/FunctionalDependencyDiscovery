#include "Tane.h"
#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

int main() {
	ofstream fout("./result.txt");

	time_t begin, end;
	begin = clock();

	cout << "============================================" << endl;
	cout << "Initializing tane algorithm . . ." << endl;
	Tane tane = Tane("./data.txt");
	cout << "Initialize ok!" << endl;
	cout << "============================================" << endl;
	cout << "Running algorithm . . ." << endl;
	tane.run(fout);
	cout << "============================================" << endl;
	cout << "Number of functional depedencies : " << tane.getCount() << endl;
	cout << "Get more detail in result.txt" << endl;

	end = clock();
	cout << "Runtime : " << double(end - begin) / CLOCKS_PER_SEC << endl;
	cout << "============================================" << endl;

	fout.flush();
	fout.close();

	return 0;
}