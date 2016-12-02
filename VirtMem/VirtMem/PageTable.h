#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <queue>

#include "memory.h"
#include "BackingStore.h"

typedef std::map<int, std::pair<int, bool>> PT;	// underlying map; maps page # to the pair <frame # (-1 if not in page table), dirty bit (1 if written to)>
typedef PT::const_iterator It;

class PageTable {
private:
	PT pt;
	std::string replacementPolicy;
	std::queue<int> q;	// for implementing FIFO or LRU page replacement
	int numPageFaults = 0;
	BackingStore* bs;
public:
	PageTable(const std::string& pageReplacementPolicy = "FIFO", const std::string& backingStorePath = "BACKING_STORE.bin");
	~PageTable();
	bool hasFreeFrames();
	int getFrameNum(int pnum);
	int getFreeFrameNum();
};