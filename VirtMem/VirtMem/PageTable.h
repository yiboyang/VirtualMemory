#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <deque>

#include "memory.h"
#include "BackingStore.h"

class PageTable {
private:
	std::map<int, std::pair<int, bool>> pt;	// page table; maps page # to <frame #, dirty bit>
	std::map<int, int> ft;	// frame table; maps frame number (physical memory) to page number (logical memory)
	std::string replacementPolicy;
	std::deque<int> q;	// a queue (deque) of used pages, for implementing FIFO or LRU page replacement
	bool pageFault = false;	// a bit indicating whether the last page access resulted in page fault
	int numPageFaults = 0;
	BackingStore* bs;
	int getFreeFrameNum();
public:
	PageTable(const std::string& pageReplacementPolicy = "FIFO", const std::string& backingStorePath = "BACKING_STORE.bin");
	~PageTable();
	bool hasFreeFrame();
	int getFrameNum(int pnum);
};