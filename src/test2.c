#include "main.h"
#include "aes.h"

#define AES_COMPRESS_SIZE		(1024*1024*1)

int test2_run()
{
	unsigned char key[16] = {1,2,3,4,5,6,7,8};
	struct AES_ctx aesCtx;
	AES_init_ctx(&aesCtx, key);
		
	uint8* block = (uint8*)memAlloc(AES_COMPRESS_SIZE);
	for(uint32 i=0; i<AES_COMPRESS_SIZE; i++)
	{
		block[i] = i&0xFF;
	}

	for(uint32 i=0; i<400; i++)
		AES_CBC_encrypt_buffer(&aesCtx, block, AES_COMPRESS_SIZE);
	
	memFree(block);

	return 0;
}