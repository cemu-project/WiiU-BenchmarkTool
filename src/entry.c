#include <string.h>
#include "main.h"

int (*OSDynLoad_Acquire)(const char* rpl, u32 *handle);
int (*OSDynLoad_FindExport)(u32 handle, int isdata, const char *symbol, void *address);

// coreinit OSScreen
void (*OSScreenInit)(void);
unsigned int (*OSScreenGetBufferSizeEx)(unsigned int bufferNum);
int (*OSScreenSetBufferEx)(unsigned int bufferNum, void * addr);
int (*OSScreenClearBufferEx)(unsigned int bufferNum, unsigned int temp);
int (*OSScreenFlipBuffersEx)(unsigned int bufferNum);
int (*OSScreenPutFontEx)(unsigned int bufferNum, unsigned int posX, unsigned int posY, const char * buffer);
void (*OSScreenPutPixelEx)(int bufferNum, uint32 posX, uint32 posY, uint32 color);
int (*OSScreenEnableEx)(unsigned int bufferNum, int enable);
// coreinit memory
void* (*MEMAllocFromDefaultHeapEx)(uint32_t size, int alignment);
void (*MEMFreeToDefaultHeap)(void* addr);

// coreinit time
u64  (*OSGetTime)(void);
void (*OSSleepTicks)(u64 ticks);

// misc
int (*_snprintf)(char *buf, int n, const char *format, ... );

int __entry_menu(int argc, char **argv)
{	
	// function offsets setup by Homebrew Launcher:
	// 0x00801500 -> OSDynLoad_Acquire
	// 0x00801504 -> OSDynLoad_FindExport
	
	OSDynLoad_Acquire = (int(*)(const char*, u32*))*(u32*)0x00801500;
	OSDynLoad_FindExport = (int (*)(u32, int, const char *, void*))*(u32*)0x00801504;
	u32 coreinitHandle;
	OSDynLoad_Acquire("coreinit", &coreinitHandle);
	// coreinit OSScreen
	OSDynLoad_FindExport(coreinitHandle, 0, "OSScreenInit", (void*)&OSScreenInit);
	OSDynLoad_FindExport(coreinitHandle, 0, "OSScreenGetBufferSizeEx", (void*)&OSScreenGetBufferSizeEx);
	OSDynLoad_FindExport(coreinitHandle, 0, "OSScreenSetBufferEx", (void*)&OSScreenSetBufferEx);
	OSDynLoad_FindExport(coreinitHandle, 0, "OSScreenClearBufferEx", (void*)&OSScreenClearBufferEx);
	OSDynLoad_FindExport(coreinitHandle, 0, "OSScreenFlipBuffersEx", (void*)&OSScreenFlipBuffersEx);
	OSDynLoad_FindExport(coreinitHandle, 0, "OSScreenPutFontEx", (void*)&OSScreenPutFontEx);
	OSDynLoad_FindExport(coreinitHandle, 0, "OSScreenPutPixelEx", (void*)&OSScreenPutPixelEx);
	OSDynLoad_FindExport(coreinitHandle, 0, "OSScreenEnableEx", (void*)&OSScreenEnableEx);
	// coreinit time
	OSDynLoad_FindExport(coreinitHandle, 0, "OSGetTime", (void*)&OSGetTime);
	// coreinit thread
	OSDynLoad_FindExport(coreinitHandle, 0, "OSSleepTicks", (void*)&OSSleepTicks);
	// coreinit memory
	void* temp;
	OSDynLoad_FindExport(coreinitHandle, 1, "MEMAllocFromDefaultHeapEx", (void*)&temp);
	*(void**)&MEMAllocFromDefaultHeapEx = *(void**)temp;
	OSDynLoad_FindExport(coreinitHandle, 1, "MEMFreeToDefaultHeap", (void*)&temp);
	*(void**)&MEMFreeToDefaultHeap = *(void**)temp;
	// coreinit misc
	OSDynLoad_FindExport(coreinitHandle, 0, "__os_snprintf", (void*)&_snprintf);
	
    int ret = mainFunc();
    return ret;
}

void* memAlloc(int size)
{
	return MEMAllocFromDefaultHeapEx(size, 16);
}

void memFree(void* ptr)
{
	MEMFreeToDefaultHeap(ptr);
}
