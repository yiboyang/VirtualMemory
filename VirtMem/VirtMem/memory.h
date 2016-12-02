#pragma once
/*
Define the page (frame) size, logical and physical memory size
*/
const int NUM_LOGICAL_MEM_FRAMES = 256;	// logical memory can address 2^8=256 frames/pages (each logical address has 16 bits)
const int NUM_PHYSICAL_MEM_FRAMES = 64;	// physical memory has only 64 frames
const int FRAME_SIZE_BITS = 8;		// lower 8 bits of logical memory address
const int FRAME_SIZE = 256;			// 2^FRAME_SIZE, same as page size (in number of bytes)
const int FRAME_OFFSET_MASK = FRAME_SIZE - 1;	// mask to preserve the lower 8 bits (255 is binary 1111 1111)
extern unsigned char Memory[NUM_PHYSICAL_MEM_FRAMES][FRAME_SIZE];	// 64*256=2^14 bytes physical memor
