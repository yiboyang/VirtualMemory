#include <iostream>
#include <map>
#include <fstream>
#include "BackingStore.h"

using namespace std;


const int FRAMESIZE = 256;			// i.e. page size
const int PHYSICALMEMFRAMES = 64;	// physical memory has only 64 frames

typedef map<int, std::pair<int, bool>> PageTable;	// maps page # to the pair <frame # (-1 if not in page table), dirty bit (1 if written to)>

unsigned char Memory[PHYSICALMEMFRAMES][FRAMESIZE];	// 2^14 bytes physical memory

int main(int argc, char** argv) {

	// for debugging
	argc = 2;
	argv[1] = "input_reads.txt";

	if (argc != 2) {
		cerr << "Usage: ./a.out [input file]\n";
		return -1;
	}

	// open backing store
	BackingStore * bs;
	try {
		bs = new BackingStore("BACKING_STORE.bin");
	}
	catch (const std::invalid_argument& e) {
		std::cerr << "Unable to open backing store file\n";
		return -1;
	}

	// open input file
	ifstream input(argv[1]);
	if (!input) {
		std::cerr << "Unable to open input file\n";
		return -1;
	}
	string line;
	while (getline(input, line)) {
		cout << line;
	}

	unsigned char test[FRAMESIZE];

	int frame = 209, offset = 179;



	bs->read(frame); // test reading frame 0
	copy(bs->getBuff(), bs->getBuff() + FRAMESIZE, test);
	// copy to test array

	cout << (int)test[offset] << test[offset];

	input.close();
	delete bs;
}
