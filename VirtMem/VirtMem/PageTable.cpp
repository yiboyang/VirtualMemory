#include <fstream>
#include <iostream>
#include <algorithm>
#include "PageTable.h"
#include "TLB.h"

using namespace std;

unsigned char Memory[NUM_PHYSICAL_MEM_FRAMES][FRAME_SIZE];	// declare the extern variable

PageTable::PageTable(const string& pageReplacementPolicy, const string& backingStorePath) : bs(backingStorePath) {
	if (!(pageReplacementPolicy == "FIFO" || pageReplacementPolicy == "LRU"))
		throw invalid_argument("Page replacement policy must be FIFO or LRU");
	else
		this->pageReplacementPolicy = pageReplacementPolicy;

	// initialize all page entries such that they all point to frame # -1 with dirty bit set to false
	for (int i = 0; i < NUM_LOGICAL_MEM_FRAMES; i++)
		pt[i] = make_pair(-1, false);

	// initialize free frames set; at first all frames are free
	for (int j = 0; j < NUM_PHYSICAL_MEM_FRAMES; j++)
		fs.insert(fs.end(), j);
}

PageTable::~PageTable() {
	// commit all dirty pages to backing store
	for (auto const& kv : pt) {
		if (kv.second.second)
			bs.write(kv.first, kv.second.first);
	}
}

int PageTable::operator[](const int pnum) {
	/*
	Get the number (index) of the frame in physical memory corresponding to given page number (pnum) in logical memory;
	this is done prior to every access to Memory. Page faults are resolved automatically.
	*/
	int fnum;
	pageFault = tlbMiss = false;
	if (tlb.contains(pnum)) // look in TLB first
		fnum = tlb[pnum];
	else { // TLB miss; look in page table
		tlbMiss = true;
		fnum = pt[pnum].first;

		if (fnum == -1) {
			// page fault!
			pageFault = true;
			numPageFaults++;
			// read the requested page from backing store into a free frame
			fnum = getFreeFrameNum();	// somehow find a free frame
			bs.read(pnum);		// read the page at pnum on backing store
			copy(bs.getBuff(), bs.getBuff() + FRAME_SIZE, Memory[fnum]);	// copy into the frame at fnum of physical memory
			pt[pnum].first = fnum;	// point the pnum enty to fnum in page table
		}

		tlb[pnum] = fnum;	// update TLB
	}


	if (pageFault) {
		// with either FIFO or LRU, if a page is newly brought into memory, its index is inserted at tail of queue
		q.push_back(pnum);
	}
	else {
		// in the case of no page fault, FIFO does nothing;
		// LRU will move the recently accessed page number to the tail of queue
		if (pageReplacementPolicy == "LRU") {
			q.remove(pnum);
			q.push_back(pnum);
		}
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
	else {	// need to replace a page, q is full (since we don't delete a page by itself, fs.size() == 64 - q.size())
		// either FIFO or LRU replaces the page at the front of the queue
		int pnum = q.front();	// victim page number
		fnum = pt[pnum].first;

		// if a dirty page, write its memory frame to backing store
		if (pt[pnum].second) {
			bs.write(pnum, fnum);
			setDirty(pnum, false);	// reset dirty bit
		}

		pt[pnum].first = -1;	// reset victim page's frame number
		q.pop_front();
	}
	return fnum;
}

// print all valid entries of page table, page # followed by frame #
ostream& operator<<(ostream &strm, const PageTable &PT) {
	for (auto const& kv : PT.pt) {
		if (kv.second.first != -1)
			strm << kv.first << ' ' << kv.second.first << endl;
	}
	return strm;
}
