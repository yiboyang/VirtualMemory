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

	// seek to the page at pnum
	if (store.seekg(pnum * FRAME_SIZE, ios::beg))
		store.read((char *)buff, FRAME_SIZE);
	else 
		throw std::runtime_error("Seek error");
}

/*
Write the physical memory frame at fnum to the page at pnum on backing store
*/
void BackingStore::write(int pnum, int fnum) {
	assert(!store.eof());

	// seek to the page at pnum
	if (store.seekg(pnum * FRAME_SIZE, ios::beg))
		store.write((char *)Memory[fnum], FRAME_SIZE);
	else 
		throw std::runtime_error("Seek error");
}
