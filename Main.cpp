#include "Tane.h"
#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

int main(int argc, char* argv[]) {
	if (argc != 3) {
		cout << "Wrong parameter input." << endl;
		return 0;
	}

	string inputFileName = argv[1];
	string outputFileName = argv[2];

	ofstream fout(outputFileName);

	time_t begin, end;
	begin = clock();

	cout << "============================================" << endl;
	cout << "Initializing tane algorithm . . ." << endl;
	Tane tane;
	try {
		tane = Tane(inputFileName);
	} catch (...) {
		cout << "Input file is not found." << endl;
		cout << "============================================" << endl;
		return 0;
	}
	cout << "Initialize ok!" << endl;
	cout << "============================================" << endl;
	cout << "Running algorithm . . ." << endl;
	tane.run(fout);
	cout << "============================================" << endl;
	cout << "Number of functional dependencies : " << tane.getCount() << endl;
	cout << "Get more detail in your output file." << endl;

	end = clock();
	cout << "Runtime : " << double(end - begin) / CLOCKS_PER_SEC << endl;
	cout << "============================================" << endl;

	fout.flush();
	fout.close();

	return 0;
}