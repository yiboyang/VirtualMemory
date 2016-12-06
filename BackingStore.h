#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include "memory.h"

class BackingStore {
private:
	std::fstream store;
	unsigned char buff[FRAME_SIZE];	// an array of unsigned char buffer (as per project requirement), holds content of a page
public:
	BackingStore(std::string name) {
		// open the file that simulates disk in rw mode
		store.open(name, std::ios::binary | std::ios::in | std::ios::out);	// c++11 allows istream.open string rather than char*
		if (!store)
			throw std::invalid_argument("Can't open backing store file!");
	}

	~BackingStore() {
		store.close();
	}

	void read(int pnum);

	void write(int pnum, int fnum);

	const unsigned char * getBuff() { return buff; }
};
