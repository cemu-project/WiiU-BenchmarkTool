#include "main.h"

__attribute__((used)) uint32 test2Result = 0;

int t4_recursiveA(sint32 depth, sint32 v0, sint32 v1)
{
	if( depth >= 200 )
		return v0 + v1;
	return t4_recursiveA(depth+1, ~v0, ~v1) + (v0>>3) + (v1>>1);	
}

int t4_recursiveB(sint32 depth, sint32 v0, sint32 v1)
{
	if( depth >= 200 )
		return v0 ^ v1;
	return t4_recursiveB(depth+1, v1, v0) + (v0>>1) + (v1>>3);
}

int t4_recursiveC(sint32 depth, sint32 v0, sint32 v1)
{
	if( depth >= 200 )
		return v0 + v1;
	return t4_recursiveC(depth+1, v0 + t4_recursiveB(0, v0, v1) + t4_recursiveA(0, v1-v0, v0>>3), v1 + t4_recursiveA(0, v0, v1) + t4_recursiveB(0, v1^v0, ~v0));
}

int test4_run()
{
	sint32 v = 0;
	for(sint32 i=0; i<2000; i++)
		v += t4_recursiveC(0, 0x11111111 + i * 133, 0xFFFFFFFF - i * 1021);

	test2Result = v;
	return 0;
}