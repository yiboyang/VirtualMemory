#pragma once
#include <fstream>
#include <iostream>
#include <string>

class BackingStore
{
private:
	std::fstream store;
	static const int FSIZE = 256;	// frame size (should equal page size)
	unsigned char fbuff[FSIZE];		// an array of unsigned char buffer (as per project requirement), holds content of a frame
public:
	BackingStore(std::string name)
	{
		// open the file that simulates disk in rw mode
		store.open(name, std::ios::binary | std::ios::in | std::ios::out);
		if (!store)
			std::cerr << "Unable to open backing store file\n";
	}

	~BackingStore()
	{
		store.close();
	}

	void read(int fnum);

	const unsigned char * getBuff() { return fbuff; }
};