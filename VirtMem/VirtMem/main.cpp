#include <iostream>
#include "BackingStore.h"

using namespace std;

int main()
{
	const int FRAMESIZE = 256;
	BackingStore * bs = new BackingStore("BACKING_STORE.bin");

	unsigned char test[FRAMESIZE];

	int frame = 209, offset = 179;



	bs->read(frame); // test reading frame 0
	copy(bs->getBuff(), bs->getBuff() + FRAMESIZE, test);
	// copy to test array

	cout << (int)test[offset] << test[offset];

	delete bs;
}
