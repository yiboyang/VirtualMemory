#include <fstream>
#include <iostream>
#include "PageTable.h"
using namespace std;

unsigned char Memory[PHYSICAL_NUM_FRAMES][FRAME_SIZE];

PageTable::PageTable(const string& pageReplacementPolicy, const string& backingStorePath) {
	if (!(pageReplacementPolicy == "FIFO" || pageReplacementPolicy == "LRU"))
		throw std::invalid_argument("Page replacement policy must be FIFO or LRU");
	else
		replacementPolicy = pageReplacementPolicy;

	// initialize such that all entries point to frame -1 and dirty bit set to false
	for (int i = 0; i < FRAME_SIZE; i++) {
		pt[i] = std::make_pair(-1, false);
	}

	// open backing store
	bs = new BackingStore(backingStorePath);
}

PageTable::~PageTable() {
	delete bs;
}

bool PageTable::hasFreeFrames() {
	return q.size() < PHYSICAL_NUM_FRAMES;	// if the queue (physical memory) is not full, there're free frames available
}

int PageTable::getFrameNum(int pnum) {
	/*
	Get the number (index) of the frame in physical memory corresponding to given page number (pnum) in logical memory
	*/
	int fnum = pt[pnum].first;
	// if this page is not in page table
	if (fnum == -1) {
		numPageFaults++;	// page fault!
		// read the requested page from backing store into a free frame
		fnum = getFreeFrameNum();	// somehow find a free frame
		bs->read(pnum);	// read the page at pnum
		copy(bs->getBuff(), bs->getBuff() + FRAME_SIZE, Memory[fnum]);	// copy into the frame at fnum of physical memory
		pt[pnum].first = fnum;	// point the pnum entry to fnum
	}
	return fnum;
}

int PageTable::getFreeFrameNum() {
	/* find a free frame and return its frame number
	*/ 
	if (hasFreeFrames()) {
		// look in the frame table to get the first free frame
		//int i = 0;
		//for (It it(pt.begin()); it != pt.end(); it++, i++) {
		//	if (it->second.first == -1)
		//		break;
		//}
		//return i;
	}
	else {	// need to replace a page
	}
	return 0;
}
