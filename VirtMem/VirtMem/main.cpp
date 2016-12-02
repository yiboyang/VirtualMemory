#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <sstream>
#include "memory.h"
#include "PageTable.h"

using namespace std;

int main(int argc, char** argv) {

	// for debugging
	argc = 2;
	argv[1] = "input.txt";

	if (argc != 2) {
		cerr << "Usage: ./a.out [input file]\n";
		return -1;
	}

	// open input file
	ifstream input(argv[1]);
	if (!input) {
		cerr << "Unable to open input file\n";
		return -1;
	}

	// create page table
	PageTable* T;
	try {
		T = new PageTable();	// set to FIFO by default
		// PageTable* T = new PageTable("LRU");
	}
	catch (const std::invalid_argument& e) {
		cerr << e.what();
		return -1;
	}


	// input line looks like
	// R 245
	// W 5421 132
	// R(ead) Address, or W(rite) Address Argument, separated by spaces
	string line, op;
	int addr, arg, pnum, offset, fnum, memval;
	vector<string> fields;
	while (getline(input, line)) {
		istringstream iss(line);
		for (string s; iss >> s; )
			fields.push_back(s);
		op = fields[0];
		addr = stoi(fields[1]);

		pnum = addr >> FRAME_SIZE_BITS;	// page number
		offset = addr & FRAME_OFFSET_MASK;	// page offset

		if (op == "R") {
			fnum = T->getFrameNum(pnum);
			memval = Memory[fnum][offset];
			cout << "Virtual address: " << addr << " Physical address: " << ((fnum << FRAME_SIZE_BITS) | offset) << " Value: " << memval << endl; 
		}
		else if (op == "W") {
			arg = stoi(fields[2]);
		}
		else {
			cerr << "Bad operation: " << op << endl;
		}


		fields.clear();
	}
	input.close();
}
