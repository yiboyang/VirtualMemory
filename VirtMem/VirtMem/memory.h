#pragma once
/*
Define the page size and physical memory
*/
const int FRAME_SIZE = 256;			// equal to page size (number of bytes)
const int FRAME_SIZE_BITS = 8;		// 2^8 = 256
const int FRAME_OFFSET_MASK = FRAME_SIZE * 2 - 1;	// mask to preserve the lower 8 bits (511 is binary 1111 1111)
const int PHYSICAL_NUM_FRAMES = 64;	// physical memory has only 64 frames
extern unsigned char Memory[PHYSICAL_NUM_FRAMES][FRAME_SIZE];	// 64*256=2^14 bytes physical memor