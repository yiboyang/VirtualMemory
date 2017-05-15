#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <sstream>
#include "memory.h"
#include "PageTable.h"

using namespace std;

int main(int argc, char** argv) {

	if (!((argc == 3) && (strcmp(argv[1], "-F") == 0 || strcmp(argv[1], "-L") == 0))) {
		cerr << "Usage: ./a.out [-F|-L] [INPUT FILE]" << endl
			<< "\t-F, use FIFO page replacement policy" << endl
			<< "\t-L, use LRU page replacment policy" << endl;
		return -1;
	}

	// create page table
	PageTable T(strcmp(argv[1], "-F") == 0 ? "FIFO" : "LRU");

	// open input file
	ifstream input(argv[2]);
	if (!input) {
		cerr << "Unable to open input file\n";
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
		fnum = T[pnum];

		if (op == "R") {
		}
		else if (op == "W") {
			T.setDirty(pnum, true);
			arg = stoi(fields[2]);
			Memory[fnum][offset] = arg;
		}
		else
			cerr << "Bad operation: " << op << endl;

		memval = Memory[fnum][offset];

		// output format: page #, offset, TLB hit, page fault, physical address, value (only for reads)
		cout << pnum << ' ' << offset << ' ' << (!T.tlbMiss ? 'H' : 'N') << ' '
			<< (T.pageFault ? 'F' : 'N') << ' ' << ((fnum << FRAME_SIZE_BITS) | offset);
		// print memory content for reads
		if (op == "R")
			cout << ' ' << memval;
		cout << endl;

		fields.clear();
	}
	cout << endl << "Page Table:" << endl << T << endl;
	cout << endl << T.numPageFaults << " Page Faults" << endl;
	input.close();
}
