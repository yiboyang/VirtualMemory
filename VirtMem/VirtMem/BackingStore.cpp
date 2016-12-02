#include <fstream>
#include <iostream>
#include <assert.h>
#include <stdexcept>

#include "BackingStore.h"

using namespace std;

/*
Read page at pnum (0 based index) into buffer
*/
void BackingStore::read(int pnum) {
	assert(!store.eof());

	// seek to the right byte position
	if (store.seekg(pnum * FRAME_SIZE, ios::beg))
		store.read((char *)buff, FRAME_SIZE);
	else {
		// std::cerr << "Seek error" << std::endl;
		throw std::runtime_error("Seek error");
	}
}