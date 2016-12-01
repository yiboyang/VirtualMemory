#pragma once
#include <fstream>
#include <iostream>
#include <assert.h>
#include <stdexcept>

#include "BackingStore.h"

using namespace std;

/*
Read frame at fnum (0 based index) into buffer
*/
void BackingStore::read(int fnum) {
	assert(!store.eof());

	// seek to the right byte position
	if (store.seekg(fnum * FRAMESIZE, ios::beg))
		store.read((char *)fbuff, FRAMESIZE);
	else {
		// std::cerr << "Seek error" << std::endl;
		throw std::runtime_error("Seek error");
	}
}