# VirtualMemory

This is a simulation of virtual memory with paging and LRU cache, based on
chapter 9 programming project of Operating System Concepts, 9th edition.

Compile: `make`

Run: `./a.out -F|-L input_file`

Must specify either -F (FIFO) or -L (LRU) page replacement policy.
Note that the TLB uses the LRU policy, memory is implemented as a char array,
and the backing storage file `BACKING_STORE.bin` mirrors the memory content.
