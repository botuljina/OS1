#include "Thread.h"
#include "Timer.h"
#include "Semaphor.h"
#include "sPrintf.h"
#include <iostream.h>
#include <stdlib.h>

/*
 	 Test: Semafori sa spavanjem
*/
volatile int theEnd=0;
const int n = 10;
int t=-1;

Semaphore s(0);
Semaphore s2(5);
class TestThread : public Thread
{
public:

	TestThread(): Thread(){}
	~TestThread()
	{
		waitToComplete();
	}
protected:

	void run();

};

void TestThread::run()
{
	syncPrintf("Thread start...\n");
	s2.wait(0);
	t=0;
	s.wait(10);
	syncPrintf("Thread finished.\n");
	//s.signal();
	s2.signal(1);
}

void tick()
{
	t++;
	if(t)
		syncPrintf("%d\n",t);
}

int userMain(int argc, char** argv)
{
	syncPrintf("Test starts.\n");
	s2.turnOnPriorities();

	TestThread t[n];
	int i;
	for(i=0;i<n;i++)
	{
		t[i].start();
	}

	syncPrintf("Test ends.\n");
	return 0;
}



