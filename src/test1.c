#include "main.h"

__attribute__((used)) uint32 test1Result = 0;

int test1_run()
{
	uint32 v0 = 0x11111111;
	uint32 v1 = 0x22222222;
	uint32 v2 = 0x33333333;
	uint32 v3 = 0x44444444;
	uint32 v4 = 0x55555555;
	uint32 v5 = 0x66666666;
	uint32 v6 = 0x77777777;
	uint32 v7 = 0x88888888;
	uint32 v8 = 0x99999999;
	uint32 v9 = 0xAAAAAAAA;
	uint32 v10 = 0xBBBBBBBB;
	uint32 v11 = 0xCCCCCCCC;
	uint32 v12 = 0xDDDDDDDD;
	uint32 v13 = 0xEEEEEEEE;
	uint32 v14 = 0xFFFFFFFF;
	uint32 v15 = 0x00000000;
	for(uint32 i=0; i<0x60000000; i++)
	{
		v0 += v1;
		v1 ^= v2;
		v2 -= (v3<<7)|(v3>>(32-7));
		v3 = (v2 - v4) ^ v3;
		v4 = ~v4 + v5;
		v5 = (v6<<7)|(v6>>(32-7));
		v6 = v7 - v4;
		v7 = (v7>>1) + ~v8;
		v8 = v9 + 0x111;
		v9 = v10 * v8 + v10;
		v10 = (v11 >> 1) + (v11 >> 20);
		v11 = v11 + v12;
		v12 = v12 + v13;
		v13 = v13 + v14;
		v14 = v14 + v15;
		v15 = v15 + v0;
	}
	test1Result = v0;
	return 0;
}