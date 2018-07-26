#include "main.h"
#include "benchmarkTests.h"

#define TEST_STATE_QUEUED		(0)
#define TEST_STATE_RUNNING		(1)
#define TEST_STATE_COMPLETED	(2)
#define TEST_STATE_FAILED		(3)

typedef struct
{
	uint64 tickStart;
	uint64 tickEnd;
	uint8 state;
	char* name;
	int(*testFunc)();
}testResultState_t;

testResultState_t testResultList[32];
sint32 testResultCount = 0;

void drawStatusScreen()
{
	OSScreenClearBufferEx(0, 0);
	OSScreenClearBufferEx(1, 0);
	drawText(-1, 1, 1, 0xFF905000, "Wii U CPU benchmark tool v0.1 (single-core)");
	drawText(-1, 1, 3, 0xFFFFFFFF, "Status      Duration    Test-Name");
	drawHorizontalLineW2(-1, 1, 4, 50, 0xFFFFFFFF);
	char text[128];
	int allComplete = 1;
	sint32 totalTime = 0;
	for(sint32 i=0; i<testResultCount; i++)
	{
		char durationText[32];
		durationText[0] = '?';
		durationText[1] = '\0';
		if( testResultList[i].state == TEST_STATE_COMPLETED )
		{
			drawText(0, 1, 5+i, 0x80FF8000, "COMPLETED");
			// calculate duration
			uint64 durationInTicks = testResultList[i].tickEnd - testResultList[i].tickStart;
			uint64 durationinMS = durationInTicks * (uint64)1000 / (uint64)TICK_SPEED;
			_snprintf(durationText, sizeof(text), "%dms", (sint32)durationinMS);
			totalTime += (sint32)durationinMS;
		}
		else if( testResultList[i].state == TEST_STATE_FAILED )
		{
			allComplete = 0;
			drawText(-1, 1, 5+i, 0xFF808000, "FAILED");
		}
		else if( testResultList[i].state == TEST_STATE_QUEUED )
		{
			allComplete = 0;
			drawText(-1, 1, 5+i, 0x80808000, "QUEUED");
		}
		else
		{
			allComplete = 0;
			drawText(-1, 1, 5+i, 0x8080FFFF, "RUNNING");
		}
		_snprintf(text, sizeof(text), "%-11s %-11s", durationText, testResultList[i].name);
		drawText(-1, 1+12, 5+i, 0xFFFFFFFF, text);
	}
	// draw total time if all tests completed
	if( allComplete != 0 )
	{
		_snprintf(text, sizeof(text), "Total: %dms", totalTime);
		drawText(-1, 1, 5+testResultCount+2, 0xFFFFFFFF, text);
	}
	OSScreenFlipBuffersEx(0);
	OSScreenFlipBuffersEx(1);
}

void drawResult()
{
	drawStatusScreen();
}

void queueTest(int(*testFunc)(), char* name)
{
	sint32 testIndex = 	testResultCount;
	testResultList[testIndex].name = name;
	testResultList[testIndex].testFunc = testFunc;
	testResultList[testIndex].tickStart = 0;
	testResultList[testIndex].tickEnd = 0;	
	testResultList[testIndex].state = TEST_STATE_QUEUED;	
	testResultCount++;
}

void runTest(int testIndex)
{
	testResultList[testIndex].state = TEST_STATE_RUNNING;
	drawStatusScreen();
	uint64 tickStart = OSGetTime();
	testResultList[testIndex].testFunc();
	uint64 tickEnd = OSGetTime();
	testResultList[testIndex].tickStart = tickStart;
	testResultList[testIndex].tickEnd = tickEnd;	
	testResultList[testIndex].state = TEST_STATE_COMPLETED;	
	drawStatusScreen();
}

int mainFunc(void)
{
	OSScreenInit();
	OSScreenSetBufferEx(0, (void*)0xF4000000);
	OSScreenSetBufferEx(1, (void*)(0xF4000000 + OSScreenGetBufferSizeEx(0)));
	OSScreenEnableEx(0, 1);
	OSScreenEnableEx(1, 1);
	OSScreenClearBufferEx(0, 0);
	OSScreenClearBufferEx(1, 0);
	OSScreenFlipBuffersEx(0);
	
	drawStatusScreen();
	
	queueTest(test1_run, "ALU"); // arithmetic in a loop
	queueTest(test2_run, "AES128"); // AES128 compression
	queueTest(test3_run, "COPY"); // memory copy
	queueTest(test4_run, "RECUR"); // recursive functions
	
	// run tests
	for(sint32 i=0; i<testResultCount; i++)
	{
		runTest(i);
	}
	while( 1 )
	{
		drawResult();
		OSSleepTicks(MILLISECS_TO_TICKS(1000/30)); // aim for roughly 30 FPS
	}
	return 0;
}

