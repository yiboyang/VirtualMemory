#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <list>

#include "memory.h"
#include "BackingStore.h"

class PageTable {
private:
	std::map<int, std::pair<int, bool>> pt;	// page table; maps page # to <frame #, dirty bit>
	std::set<int> fs;	// a set of free frame indices ("frame table")
	std::string replacementPolicy;
	std::list<int> q;	// a queue (deque) of used pages, for implementing FIFO or LRU page replacement
	bool pageFault = false;	// a bit indicating whether the last page access resulted in page fault
	int numPageFaults = 0;
	BackingStore* bs;
	int getFreeFrameNum();
public:
	PageTable(const std::string& pageReplacementPolicy = "FIFO", const std::string& backingStorePath = "BACKING_STORE.bin");
	~PageTable();
	int getFrameNum(int pnum);
	void setDirty(int pnum, bool dirty) { pt[pnum].second = dirty; }
};
