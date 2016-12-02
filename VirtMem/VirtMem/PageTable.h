#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <queue>

#include "memory.h"
#include "BackingStore.h"

typedef std::map<int, std::pair<int, bool>> PT;	// underlying map for page table; maps page # to the pair <frame # (-1 if not in page table), dirty bit (1 if written to)>
typedef std::map<int, int> FT; // map for frame table; maps frame number (physical memory) to page number (logical memory)
// typedef PT::const_iterator It;

class PageTable {
private:
	PT pt;
	FT ft;	// uses an auxilary frame table for processing
	std::string replacementPolicy;
	std::queue<int> q;	// a queue of used frames; for implementing FIFO or LRU page replacement
	int numPageFaults = 0;
	BackingStore* bs;
public:
	PageTable(const std::string& pageReplacementPolicy = "FIFO", const std::string& backingStorePath = "BACKING_STORE.bin");
	~PageTable();
	bool hasFreeFrames();
	int getFrameNum(int pnum);
	int getFreeFrameNum();
};