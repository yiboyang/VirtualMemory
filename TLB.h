#pragma once
#include <map>
#include <list>
#include "memory.h"
/*
Translation lookaside buffer, a cache for page table
(not really needed in this page table implementation, just for demo purpose)
*/
class TLB {
private:
	std::map<int, int> tb;	// underlying table mapping page number to frame number; will be a subset of page table
	std::list<int> q;	// TLB has its own queue of used pages and implements LRU
public:
	int& operator[] (const int pnum);
	int contains(const int pnum) { return tb.count(pnum); }
};
