#include <fstream>
#include <iostream>
#include "PageTable.h"
using namespace std;

unsigned char Memory[NUM_PHYSICAL_MEM_FRAMES][FRAME_SIZE];

PageTable::PageTable(const string& pageReplacementPolicy, const string& backingStorePath) {
	if (!(pageReplacementPolicy == "FIFO" || pageReplacementPolicy == "LRU"))
		throw std::invalid_argument("Page replacement policy must be FIFO or LRU");
	else
		replacementPolicy = pageReplacementPolicy;

	// initialize all page entries such that they all point to frame -1 with dirty bit set to false
	for (int i = 0; i < NUM_LOGICAL_MEM_FRAMES; i++) {
		pt[i] = std::make_pair(-1, false);
	}

	// initialize frame table
	for (int j = 0; j < NUM_PHYSICAL_MEM_FRAMES; j++) {
		ft[j] = -1;
	}

	// open backing store
	bs = new BackingStore(backingStorePath);
}

PageTable::~PageTable() {
	delete bs;
}

bool PageTable::hasFreeFrames() {
	/*
	Tell if there's any free frame left; the queue size is the number of occupied frames (i.e. entries in ft whose value >=0), so
	if the queue is full, the physical memory (all frames) is full
	*/
	return q.size() < NUM_PHYSICAL_MEM_FRAMES;
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
		pt[pnum].first = fnum;	// point the pnum enty to fnum in page table
		ft[fnum] = pnum;		// point the fnum to pnum in frame table
		q.push(fnum);
	}
	return fnum;
}

int PageTable::getFreeFrameNum() {
	/* find a free frame and return its frame number
	*/ 
	if (hasFreeFrames()) {
		// look in the frame table to get the first free frame (linear scan, naive)
		for (auto const& kv : ft) {
			if (kv.second == -1)
				return kv.first;
		}
	}
	else {	// need to replace a page
	}
	return -2;
}
