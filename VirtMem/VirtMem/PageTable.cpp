#include <fstream>
#include <iostream>
#include <assert.h>	// for debugging
#include "PageTable.h"
using namespace std;

unsigned char Memory[NUM_PHYSICAL_MEM_FRAMES][FRAME_SIZE];	// declare the extern variable

PageTable::PageTable(const string& pageReplacementPolicy, const string& backingStorePath) {
	if (!(pageReplacementPolicy == "FIFO" || pageReplacementPolicy == "LRU"))
		throw std::invalid_argument("Page replacement policy must be FIFO or LRU");
	else
		replacementPolicy = pageReplacementPolicy;

	// initialize all page entries such that they all point to frame # -1 with dirty bit set to false
	for (int i = 0; i < NUM_LOGICAL_MEM_FRAMES; i++)
		pt[i] = std::make_pair(-1, false);

	// initialize free frames set; at first all frames are free
	for (int j = 0; j < NUM_PHYSICAL_MEM_FRAMES; j++)
		fs.insert(fs.end(), j);

	// open backing store
	bs = new BackingStore(backingStorePath);
}

PageTable::~PageTable() {
	// commit all dirty pages to backing store
	for (auto const& kv : pt) {
		if (kv.second.second)
			bs->write(kv.first, kv.second.first);
	}
	delete bs;
}

int PageTable::getFrameNum(int pnum) {
	/*
	Get the number (index) of the frame in physical memory corresponding to given page number (pnum) in logical memory;
	this is done prior to every access to Memory. Page faults are resolved automatically.
	*/
	int fnum = pt[pnum].first;
	// if this page is not in page table
	if (fnum == -1) {
		pageFault = true;
		numPageFaults++;	// page fault!
		// read the requested page from backing store into a free frame
		fnum = getFreeFrameNum();	// somehow find a free frame
		bs->read(pnum);		// read the page at pnum on backing store
		copy(bs->getBuff(), bs->getBuff() + FRAME_SIZE, Memory[fnum]);	// copy into the frame at fnum of physical memory
		pt[pnum].first = fnum;	// point the pnum enty to fnum in page table
	}
	else
		pageFault = false;

	// with either FIFO or LRU, if a page is newly brought into memory, its index is inserted at tail of queue
	if (pageFault)
		q.push_back(pnum);
	else if (replacementPolicy == "LRU") {
		// in the case of no page fault, FIFO does nothing;
		// LRU will move the recently accessed page number to the tail of queue
		list<int>::const_iterator pos = find(q.begin(), q.end(), pnum);
		assert(pos != q.end());	// pos must be in queue (as queue must contain all page numbers whose frames are in memory)
		q.erase(pos);
		q.push_back(pnum);
	}

	return fnum;
}

int PageTable::getFreeFrameNum() {
	/*
	Find a free frame and return its frame number
	*/
	int fnum;
	if (!fs.empty()) {
		fnum = *fs.begin();		// always get the free frame with the smallest frame number
		fs.erase(fnum);			// remove fnum from the set of free frames
	}
	else {	// need to replace a page
		// either FIFO or LRU replaces the page at the front of the queue
		int pnum = q.front();	// victim page number
		fnum = pt[pnum].first;
		assert(fnum != -1);

		// if a dirty page, write its memory frame to backing store
		if (pt[pnum].second) {
			bs->write(fnum, pnum);
			setDirty(pnum, false);	// reset dirty bit
		}

		pt[pnum].first = -1;	// reset victim page's frame number
		q.pop_front();
	}
	return fnum;
}
