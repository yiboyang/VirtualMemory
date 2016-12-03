#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "memory.h"
#include "PageTable.h"

using namespace std;

int main(int argc, char** argv) {

	// for debugging
	argc = 2;
	argv[1] = "input_reads.txt";

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
		// T = new PageTable();	// set to FIFO by default
		T = new PageTable("LRU");
	}
	catch (const std::invalid_argument& e) {
		cerr << e.what();
		return -1;
	}


	// input line looks like
	// R(ead) Address, or W(rite) Address Argument, separated by spaces. e.g.
	// R 245
	// W 5421 132
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

		// access the page and get its frame number
		fnum = (*T)[pnum];

		if (op == "R") {
		}
		else if (op == "W") {
			T->setDirty(pnum, true);
			arg = stoi(fields[2]);
			Memory[fnum][offset] = arg;
		}
		else
			cerr << "Bad operation: " << op << endl;

		memval = Memory[fnum][offset];

		// output format: page #, offset, TLB hit, page fault, physical address, value
		cout << pnum << ' ' << offset << ' ' << (!T->tlbMiss ? 'H' : 'N') << ' '
			<< (T->pageFault ? 'F' : 'N') << ' ' << ((fnum << FRAME_SIZE_BITS) | offset)
			<< ' ' << memval << endl;

		fields.clear();
	}
	input.close();
	delete T;
}
