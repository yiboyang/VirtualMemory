#pragma once
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

class BackingStore
{
private:
	fstream store;
	static const int FSIZE = 256;	// frame size (should equal page size)
	signed char fbuff[FSIZE];	// an array of signed char buffer, holds 
public:
	BackingStore(string name)
	{
		store.open(name, ios::binary | ios::in | ios::out);	// open the file that simulates disk in rw mode
		if (!store)
			cerr << "Unable to open backing store file\n";
	}

	BackingStore()
	{
		store.close();
	}

	void read(int fnum);

};
