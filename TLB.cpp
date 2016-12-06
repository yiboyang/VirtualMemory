#include "TLB.h"

int& TLB::operator[] (const int pnum) {
	// before returning a reference to tb[pnum], update the queue according to LRU policy
	if (tb.count(pnum))
		// if pnum is in tb
		q.remove(pnum);
	else {
		// TLB miss; tb[pnum] will create a new entry in tb and reference 0 (default int)
		if (q.size() == TLB_SIZE) {
			// if queue (TLB) full
			int victim = q.front();	// victim page number
			tb.erase(tb.find(victim));	// remove victim entry
			q.pop_front();
		}
	}
	q.push_back(pnum);	// add pnum at the end of queue

	return tb[pnum];
}
