#pragma once
#include <fstream>
#include <iostream>

#include <assert.h>

#include "BackingStore.h"


/*
Read frame at fnum (0 based index) into buffer
*/
void BackingStore::read(int fnum)
{
	// seek to the right byte position
	assert(!store.eofbit);

	if (store.seekg(fnum * FSIZE, ios::beg))
	{
		store.read((char *)fbuff, FSIZE);	// the original pointer should be preserved; i.e. we should get a signed char

	}
	else
	{
		std::cerr << "Seek error" << std::endl;
		return;
	}

	//if (fread(fbuff, sizeof(signed char), FSIZE, &store) == 0) {
	//	fprintf(stderr, "Error reading from backing store\n");
	//}

}