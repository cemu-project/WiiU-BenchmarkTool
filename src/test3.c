#include "main.h"

#define BLOCK_SIZE	(1024*1024*8) // 8MB

int test3_run()
{
	uint8* block0 = (uint8*)memAlloc(BLOCK_SIZE);
	uint8* block1 = (uint8*)memAlloc(BLOCK_SIZE);
	for(uint32 i=0; i<BLOCK_SIZE; i++)
	{
		block0[i] = i&0xFF;
	}

	for(uint32 i=0; i<2000; i++)
	{
		for(uint32 c=0; c<BLOCK_SIZE; c++)
			block1[c] = block0[c];
	}

	memFree(block0);
	memFree(block1);

	return 0;
}