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

bool PageTable::hasFreeFrame() {
	/*
	Tell if there's any free frame left; the queue size is the number of occupied frames (i.e. entries in ft whose value >=0), so
	if the queue is full, then the physical memory (all frames) is full
	*/
	return q.size() < NUM_PHYSICAL_MEM_FRAMES;
}

int PageTable::getFrameNum(int pnum) {
	/*
	Get the number (index) of the frame in physical memory corresponding to given page number (pnum) in logical memory;
	this is done prior to every access to Memory
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
		ft[fnum] = pnum;		// point fnum to pnum in frame table

	}
	else
		pageFault = false;

	// with either FIFO or LRU, if a page is newly brought into memory, its index is inserted at tail of queue
	if (pageFault)
		q.push_back(pnum);
	else if (replacementPolicy == "LRU") {
		// in the case of no page fault, FIFO does nothing;
		// LRU will move the recently accessed page number to the tail of queue
		deque<int>::const_iterator pos = find(q.begin(), q.end(), pnum);
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
	if (hasFreeFrame()) {
		// scan the frame table to for a free frame (naive)
		for (auto const& kv : ft) {
			if (kv.second == -1) {
				fnum = kv.first;
				break;
			}
		}
	}
	else {	// need to replace a page
		// both FIFO and LRU pops and returns the page # at the front of the queue 
		fnum = pt[q.front()].first;
		assert(fnum != -1);

		//// transfer a written page to backing store
		//if (pt[q.front()].second) {

		//}

		q.pop_front();
	}
	return fnum;
}
