#pragma once
#include <fstream>
#include <iostream>

#include <assert.h>

#include "BackingStore.h"

using namespace std;

/*
Read frame at fnum (0 based index) into buffer
*/
void BackingStore::read(int fnum)
{
	assert(!store.eof());

	// seek to the right byte position
	if (store.seekg(fnum * FSIZE, ios::beg))
	{
		store.read((char *)fbuff, FSIZE);
	}
	else
	{
		std::cerr << "Seek error" << std::endl;
		return;
	}

}