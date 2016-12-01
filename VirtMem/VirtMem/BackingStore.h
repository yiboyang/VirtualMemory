#pragma once
#include <fstream>
#include <iostream>
#include <string>

class BackingStore {
private:
	std::fstream store;
	static const int FRAMESIZE = 256;	// frame size (should equal page size)
	unsigned char fbuff[FRAMESIZE];		// an array of unsigned char buffer (as per project requirement), holds content of a frame
public:
	BackingStore(std::string name) {
		// open the file that simulates disk in rw mode
		store.open(name, std::ios::binary | std::ios::in | std::ios::out);	// c++11 allows istream.open string rather than char*
		if (!store)
			throw std::invalid_argument("Can't open file!");
	}

	~BackingStore() {
		store.close();
	}

	void read(int fnum);

	const unsigned char * getBuff() { return fbuff; }
};